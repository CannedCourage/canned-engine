#include "Graphics/VulkanAllocation.h"
#include "System/System.h"

int VK_FindMemoryTypeIndex( const unsigned int memoryTypeBits, const bool needHostVisible )
{
	return -1;
}

VulkanMemoryPool::VulkanMemoryPool( const GraphicsVK& Context, const unsigned int ID, const unsigned int MemoryTypeBits, const VkDeviceSize Size, const bool HostVisible ) : Context( Context ), ID( ID ), Size( Size ), HostVisible( HostVisible )
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
	memoryAllocateInfo.allocationSize = Size;
	memoryAllocateInfo.memoryTypeIndex = MemoryTypeIndex;

	//Need access to logical device
	VERIFY( VK_SUCCESS == vkAllocateMemory( Context.LogicalDevice, &memoryAllocateInfo, NULL, &DeviceMemory ) );

	if( DeviceMemory = VK_NULL_HANDLE )
	{
		return false;
	}

	if( HostVisible )
	{
		VERIFY( VK_SUCCESS == vkMapMemory( Context.LogicalDevice, DeviceMemory, 0, Size, 0, (void**)&Data ) );
	}

	// vkBlock newBlock{};
	// newBlock.Size = m_size;
	// newBlock.Offset = 0;
	// newBlock.Free = true;

	return true;
}

bool VulkanMemoryPool::Allocate( const unsigned int Size, const unsigned int Align, vkAllocation& Allocation )
{
	return true;
}

void VulkanMemoryPool::Shutdown()
{
	if( HostVisible )
	{
		vkUnmapMemory( Context.LogicalDevice, DeviceMemory );
	}

	vkFreeMemory( Context.LogicalDevice, DeviceMemory, NULL );

	for( vkBlock block : Blocks )
	{
		//Delete block somehow?
	}
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
		TRACE( "Could not allocate new memory pool." );
		throw std::runtime_error( "Could not allocate new memory pool." );
	}

	//Allocate from new pool
	pool->Allocate( Size, Align, allocation );

	return allocation;
}

bool VulkanAllocator::AllocateFromPools( const unsigned int Size, const unsigned int Align, const unsigned int MemoryTypeBits, const bool HostVisible, vkAllocation& Allocation )
{
	const VkPhysicalDeviceMemoryProperties& physicalMemoryProperties = Context.GPUInfo->MemoryProperties;

	const VkMemoryPropertyFlags required =  HostVisible ? VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT : 0;

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