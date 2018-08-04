#include "Graphics/VulkanAllocation.h"
#include "Graphics/GraphicsVK.h"

#include <algorithm>

///VULKAN MEMORY POOL///

VulkanMemoryPool::VulkanMemoryPool( GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const VkMemoryPropertyFlags RequiredProperties, const VkMemoryPropertyFlags PreferredProperties ) : Context( Context ), PoolID( ID ), PoolSize( Size * 1000 * 1000 )
{
	VERIFY( VK_SUCCESS == Context.FindMemoryTypeIndex( MemoryTypeBits, RequiredProperties, PreferredProperties, MemoryTypeIndex, MemoryProperties ) );
}

VulkanMemoryPool::~VulkanMemoryPool( void )
{
	//Move cleanup call here
}

bool VulkanMemoryPool::Init( void )
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

	if( DeviceMemory == VK_NULL_HANDLE )
	{
		return false;
	}

	if( MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
	{
		VERIFY( VK_SUCCESS == vkMapMemory( Context.LogicalDevice, DeviceMemory, 0, PoolSize, 0, (void**)&Data ) );
	}

	//Create a single block that initially contains all the memory represented by this pool
	vkBlock block = { PoolSize };

	Blocks.push_back( block );

	return true;
}

void VulkanMemoryPool::CleanUp( void )
{
	Blocks.clear();

	if( MemoryProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT )
	{
		vkUnmapMemory( Context.LogicalDevice, DeviceMemory );
	}

	vkFreeMemory( Context.LogicalDevice, DeviceMemory, NULL );
}

//TODO: bufferImageGranularity???
bool VulkanMemoryPool::Allocate( const unsigned int RequiredSize, const unsigned int Align, vkAllocation& Allocation )
{
	for( auto it = Blocks.begin(); it != Blocks.end(); it++ )
	{
		vkBlock& freeBlock = (*it);

		if( !( freeBlock.Free ) ){ continue; }

		int alignDiff = Align - ( freeBlock.Offset % Align );

		if( alignDiff == Align )
		{
			alignDiff = 0;
		}

		int adjustedSize = freeBlock.Size - alignDiff;
		int adjustedOffset = freeBlock.Offset + alignDiff;

		if( adjustedSize < RequiredSize ){ continue; }

		if( alignDiff > 0  )
		{
			//Add a block representing the unsused space between the last allocation and the new ALIGNED allocation
			vkBlock padding = {};

			padding.Size = alignDiff;
			padding.Offset = freeBlock.Offset;
			padding.ID = -1;
			padding.Free = true;

			Blocks.insert( it, padding );

			//Move the unallocated block offset onto alignment boundary
			freeBlock.Size = adjustedSize;
			freeBlock.Offset = adjustedOffset;
		}

		//Allocate the first RequiredSize bytes of the unallocated block to the new block
		vkBlock allocatedBlock = { RequiredSize, freeBlock.Offset, NextBlockID++, false };

		Blocks.insert( it, allocatedBlock );

		//Track allocated space
		Allocated += allocatedBlock.Size;

		//Move the start of the unallocated space to the end of the new block
		freeBlock.Size -= allocatedBlock.Size;
		freeBlock.Offset += allocatedBlock.Size;
		freeBlock.Free = true;

		//Fill the allocation struct
		Allocation.Data = Data + allocatedBlock.Offset;
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
	if( ( Allocation.PoolID != PoolID ) || ( Allocation.BlockID == -1 ) ){ return; }

	unsigned int BlockID = Allocation.BlockID;

	auto result = std::find_if( Blocks.begin(), Blocks.end(), [BlockID]( vkBlock& block ) -> bool
	{
		return block.ID == BlockID;
	});

	result->ID = -1;
	result->Free = true; //TODO: How/when is this merged with it's free neighbours (if any)?

	//Track allocated space
	Allocated -= result->Size;

	Allocation.Data = nullptr;
	Allocation.DeviceMemory = VK_NULL_HANDLE;
	Allocation.Offset = 0;
	Allocation.Size = 0;
	Allocation.BlockID = -1;
	Allocation.PoolID = -1;
}

///VULKAN ALLOCATOR///

VulkanAllocator::VulkanAllocator( GraphicsVK& Context ) : Context( Context )
{
	// for( int ii = 0; ii < Context.bufferCount; ii++ )
	// {
	// 	std::vector<vkAllocation> temp;
	// 	Garbage.push_back( temp );
	// }
}

void VulkanAllocator::Init( void )
{
	//NOTHING JUST NOW
}

void VulkanAllocator::CleanUp( void )
{
	for( VulkanMemoryPool* pool : Pools )
	{
		pool->CleanUp();
	}
}

vkAllocation VulkanAllocator::Allocate( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const VkMemoryPropertyFlags RequiredProperties, const VkMemoryPropertyFlags PreferredProperties )
{
	vkAllocation allocation;

	if( AllocateFromPools( Size, Align, MemoryTypeBits, RequiredProperties, PreferredProperties, allocation ) )
	{
		return allocation;
	}

	VkDeviceSize poolSize = ( RequiredProperties & VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT) ? HostVisibleMemoryMB : DeviceLocalMemoryMB;

	VulkanMemoryPool* pool = new VulkanMemoryPool( Context, NextPoolID++, MemoryTypeBits, poolSize, RequiredProperties, PreferredProperties );

	if( pool->Init() )
	{
		Pools.push_back( pool );
	}
	else
	{
		throw std::runtime_error( "Could not allocate new memory pool." );
	}

	//Allocate from new pool
	if( pool->Allocate( Size, Align, allocation ) )
	{
		return allocation;
	}
	else
	{
		throw std::runtime_error( "Could not allocate memory." );
	}
}

bool VulkanAllocator::AllocateFromPools( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const VkMemoryPropertyFlags RequiredProperties, const VkMemoryPropertyFlags PreferredProperties, vkAllocation& Allocation )
{
	const VkPhysicalDeviceMemoryProperties& physicalMemoryProperties = Context.GPUInfo->MemoryProperties;

	for( VulkanMemoryPool* pool : Pools )
	{
		const unsigned int memoryTypeIndex = pool->MemoryTypeIndex;

		// if( HostVisible && ( pool->HostVisible == false ) ){ continue; }

		if( ( ( MemoryTypeBits >> memoryTypeIndex ) & 1 ) == 0 ){ continue; }

		//const VkMemoryPropertyFlags properties = physicalMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;

		if( ( pool->MemoryProperties & RequiredProperties ) != RequiredProperties ){ continue; }

		if( ( pool->MemoryProperties & PreferredProperties ) != PreferredProperties ){ continue; }

		if( pool->Allocate( Size, Align, Allocation ) )
		{
			return true;
		}
	}

	for( VulkanMemoryPool* pool : Pools )
	{
		const unsigned int memoryTypeIndex = pool->MemoryTypeIndex;

		// if( HostVisible && ( pool->HostVisible == false ) ){ continue; }

		if( ( ( MemoryTypeBits >> memoryTypeIndex ) & 1 ) == 0 ){ continue; }

		// const VkMemoryPropertyFlags properties = physicalMemoryProperties.memoryTypes[memoryTypeIndex].propertyFlags;

		if( ( pool->MemoryProperties & RequiredProperties ) != RequiredProperties ){ continue; }

		if( pool->Allocate( Size, Align, Allocation ) )
		{
			return true;
		}
	}

	return false;
}

void VulkanAllocator::Free( vkAllocation& Allocation )
{
	if( ( Allocation.PoolID == -1 ) ){ return; }

	unsigned int PoolID = Allocation.PoolID;

	auto result = std::find_if( Pools.begin(), Pools.end(), [PoolID]( VulkanMemoryPool* Pool ) -> bool
	{
		return Pool->PoolID == PoolID;
	});

	(*result)->Free( Allocation );
}

void VulkanAllocator::EmptyGarbage( void )
{
	//NOTHING JUST NOW
}