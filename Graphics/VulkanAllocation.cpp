#include "Graphics/VulkanAllocation.h"
#include "System/System.h"
#include <algorithm>

int VK_FindMemoryTypeIndex( const unsigned int memoryTypeBits, const bool needHostVisible )
{
	return -1;
}

VulkanMemoryPool::VulkanMemoryPool( const GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible ) : Context( Context ), PoolID( ID ), PoolSize( Size ), HostVisible( HostVisible )
{
	MemoryTypeIndex = VK_FindMemoryTypeIndex( MemoryTypeBits, HostVisible ); //???
}

bool VulkanMemoryPool::Init()
{
	if( MemoryTypeIndex == UINT64_MAX )
	{
		return false;
	}

	VkMemoryAllocateInfo memoryAllocateInfo = {};

	memoryAllocateInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
	memoryAllocateInfo.allocationSize = PoolSize;
	memoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;

	//Need access to logical device
	VERIFY( VK_SUCCESS == vkAllocateMemory( Context.LogicalDevice, &memoryAllocateInfo, NULL, &DeviceMemory ) );

	if( DeviceMemory = VK_NULL_HANDLE )
	{
		return false;
	}

	if( HostVisible )
	{
		VERIFY( VK_SUCCESS == vkMapMemory( Context.LogicalDevice, DeviceMemory, 0, PoolSize, 0, (void**)&Data ) );
	}

	//Create a single block that initially contains all the memory represented by this pool
	vkBlock block = { PoolSize };

	Blocks.push_back( block );

	return true;
}

void VulkanMemoryPool::Shutdown()
{
	if( HostVisible )
	{
		vkUnmapMemory( Context.LogicalDevice, DeviceMemory );
	}

	vkFreeMemory( Context.LogicalDevice, DeviceMemory, NULL );
}

bool VulkanMemoryPool::Allocate( const unsigned int Size, const unsigned int Align, vkAllocation& Allocation )
{
	for( auto it = Blocks.begin(); it != Blocks.end(); it++ )
	{
		vkBlock& freeBlock = (*it);

		int alignRemainder = ( freeBlock.Offset % Align );
		int alignDiff = Align - alignRemainder;
		int adjustedSize = freeBlock.Size;
		int adjustedOffset = freeBlock.Offset;
		
		if( alignDiff != Align )
		{
			adjustedSize -= alignDiff;
			adjustedOffset += alignDiff;
		}

		if( !( freeBlock.Free ) && ( adjustedSize < Size ) ){ continue; }

		//Move the unallocated block offset onto alignment boundary
		freeBlock.Size = adjustedSize;
		freeBlock.Offset = adjustedOffset;

		//Allocate the first Size bytes of the unallocated block to the new block
		vkBlock allocatedBlock = { Size, freeBlock.Offset, NextBlockID++, false };

		Blocks.insert( it, allocatedBlock );

		//Move the start of the unallocated space to the end of the new block
		freeBlock.Size -= allocatedBlock.Size;
		freeBlock.Offset += Size;
		freeBlock.Free = true;

		//Fill the allocation struct
		Allocation.Data = Data;
		Allocation.DeviceMemory = DeviceMemory;
		Allocation.Offset = allocatedBlock.Offset;
		Allocation.Size = allocatedBlock.Size;
		Allocation.BlockID = allocatedBlock.ID;
		Allocation.PoolID = PoolID;

		return true;
	}

	return false;
}

void VulkanMemoryPool::Free( vkAllocation& Allocation )
{
	unsigned int BlockID = Allocation.BlockID;

	auto result = std::find_if( Blocks.begin(), Blocks.end(), [BlockID]( vkBlock& block ) -> bool
	{
		return block.ID == BlockID;
	});

	result->ID = 0;
	result->Free = true; //How/when is this merged with it's free neighbours (if any)?

	Allocation.Data = nullptr;
	Allocation.DeviceMemory = VK_NULL_HANDLE;
	Allocation.Offset = 0;
	Allocation.Size = 0;
	Allocation.BlockID = 0;
	Allocation.PoolID = 0;
}

VulkanAllocator::VulkanAllocator( const GraphicsVK& Context ) : Context( Context ), Garbage{Context.bufferCount}
{
}

vkAllocation VulkanAllocator::Allocate( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible )
{
	vkAllocation allocation;

	if( AllocateFromPools( Size, Align, MemoryTypeBits, HostVisible, allocation ) )
	{
		return allocation;
	}

	VkDeviceSize poolSize = HostVisible ? HostVisibleMemoryMB : DeviceLocalMemoryMB;

	VulkanMemoryPool* pool = new VulkanMemoryPool( Context, NextPoolID++, MemoryTypeBits, poolSize, HostVisible );

	if( pool->Init() )
	{
		Pools.push_back( pool );
	}
	else
	{
		throw std::runtime_error( "Could not allocate new memory pool." );
	}

	//Allocate from new pool
	pool->Allocate( Size, Align, allocation );

	return allocation;
}

bool VulkanAllocator::AllocateFromPools( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible, vkAllocation& Allocation )
{
	const VkPhysicalDeviceMemoryProperties& physicalMemoryProperties = Context.GPUInfo->MemoryProperties;

	const VkMemoryPropertyFlags required = HostVisible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;

	const VkMemoryPropertyFlags preferred = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;

	for( VulkanMemoryPool* pool : Pools )
	{
		const unsigned int memoryTypeIndex = pool->MemoryTypeIndex;

		if( HostVisible && ( pool->HostVisible == false ) ){ continue; }

		if( ( ( MemoryTypeBits >> memoryTypeIndex ) & 1 ) == 0 ){ continue; }

		const VkMemoryPropertyFlags properties = physicalMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;

		if( ( properties & required ) != required ){ continue; }

		if( ( properties & preferred ) != preferred ){ continue; }

		if( pool->Allocate( Size, Align, Allocation ) )
		{
			return true;
		}
	}

	for( VulkanMemoryPool* pool : Pools )
	{
		const unsigned int memoryTypeIndex = pool->MemoryTypeIndex;

		if( HostVisible && ( pool->HostVisible == false ) ){ continue; }

		if( ( ( MemoryTypeBits >> memoryTypeIndex ) & 1 ) == 0 ){ continue; }

		const VkMemoryPropertyFlags properties = physicalMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;

		if( ( properties & required ) != required ){ continue; }

		if( pool->Allocate( Size, Align, Allocation ) )
		{
			return true;
		}
	}

	return false;
}