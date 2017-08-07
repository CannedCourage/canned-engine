#include "Graphics/VulkanAllocation.h"
#include "System/System.h"
#include "Graphics/GraphicsVK.h"

#include <algorithm>
#include <bitset>

///VULKAN MEMORY POOL///

VulkanMemoryPool::VulkanMemoryPool( GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible ) : Context( Context ), PoolID( ID ), PoolSize( Size * 1000 ), HostVisible( HostVisible )
{
	VERIFY( VK_SUCCESS == FindMemoryTypeIndex( MemoryTypeBits, HostVisible, MemoryTypeIndex ) );
}

VkResult VulkanMemoryPool::FindMemoryTypeIndex( const unsigned int MemoryTypeBits, const bool NeedHostVisible, unsigned int& SelectedMemoryTypeIndex )
{
	ASSERT( MemoryTypeBits != 0 );

	const VkPhysicalDeviceMemoryProperties& physicalMemoryProperties = Context.GPUInfo->MemoryProperties;

	const VkMemoryPropertyFlags required = NeedHostVisible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;

	const VkMemoryPropertyFlags preferred = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT | required;

	//preferred must be a superset of required.
    ASSERT( ( required & ~preferred ) == 0 );

    SelectedMemoryTypeIndex = UINT32_MAX;
    unsigned int minCost = UINT32_MAX;

    //Search for the memory type that satisfies ALL the required flags, and the MOST preferred flags
    for( unsigned int memTypeIndex = 0, memTypeBit = 1; memTypeIndex < physicalMemoryProperties.memoryTypeCount; memTypeIndex++, memTypeBit <<= 1 )
    {
    	if( ( memTypeBit & MemoryTypeBits ) != 0 )
    	{
    		const VkMemoryPropertyFlags currentFlags = physicalMemoryProperties.memoryTypes[memTypeIndex].propertyFlags;

    		//If this memory type has all the required flags
    		if( ( required & ~currentFlags ) == 0 )
    		{
    			//Determine how many preferred flags it doesn't have
    			std::bitset<sizeof(VkMemoryPropertyFlags)> missingFlags(preferred & ~currentFlags);
    			unsigned int currentCost = missingFlags.count();

    			//Min search for memory type with fewest missing preferred flags
    			if( currentCost < minCost )
    			{
    				SelectedMemoryTypeIndex = memTypeIndex;

    				if( currentCost == 0 )
    				{
    					//Can't get better than this
    					return VK_SUCCESS;
    				}

    				minCost = currentCost;
    			}
    		}
    	}
    }

    //Didn't find an exact match, SelectedMemoryTypeIndex is now the closest

    //Check that any match was actually found
	return ( (SelectedMemoryTypeIndex != UINT32_MAX) ? VK_SUCCESS : VK_ERROR_FEATURE_NOT_PRESENT );
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

void VulkanMemoryPool::CleanUp( void )
{
	if( HostVisible )
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
	if( ( Allocation.PoolID != PoolID ) || ( Allocation.BlockID == -1 ) ){ return; }

	unsigned int BlockID = Allocation.BlockID;

	auto result = std::find_if( Blocks.begin(), Blocks.end(), [BlockID]( vkBlock& block ) -> bool
	{
		return block.ID == BlockID;
	});

	result->ID = -1;
	result->Free = true; //How/when is this merged with it's free neighbours (if any)?

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
	//NOTHING JUST NOW
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