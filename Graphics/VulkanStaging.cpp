#include "Graphics/VulkanStaging.h"
#include "Graphics/GraphicsVK.h"
#include "Logging/Trace.h"

#include <algorithm>
#include <stdexcept>

VulkanStagingManager::VulkanStagingManager( GraphicsVK& Context ) : Context( Context )
{
}

void VulkanStagingManager::Init( void )
{
	CreateBuffersAndMemory();

	CreateCommandPool();

	CreateCommandBuffers();

	BeginRecording();
}

void VulkanStagingManager::CreateBuffersAndMemory( void )
{
	StagingBuffers.resize( 2 );

	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = DefaultBufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
	//fenceCreateInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

	VkMemoryRequirements memRequirements;

	for( auto& buffer : StagingBuffers )
	{
		VERIFY( VK_SUCCESS == vkCreateBuffer( Context.LogicalDevice, &bufferInfo, nullptr, &( buffer.Buffer ) ) );

		VERIFY( VK_SUCCESS == vkCreateFence( Context.LogicalDevice, &fenceCreateInfo, nullptr, &( buffer.Fence ) ) );
	}

	vkGetBufferMemoryRequirements( Context.LogicalDevice, StagingBuffers[0].Buffer, &memRequirements );

	vkGetBufferMemoryRequirements( Context.LogicalDevice, StagingBuffers[1].Buffer, &memRequirements );

	MemoryPool = std::make_unique<VulkanMemoryPool>( Context, 0, memRequirements.memoryTypeBits, DefaultBufferSizeMB * 2, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0 );

	if( !( MemoryPool->Init() ) )
	{
		TRACE( "Could not allocate new memory pool" );
		throw std::runtime_error( "Could not allocate new memory pool." );
	}

	for( auto& buffer : StagingBuffers )
	{
		if( !( MemoryPool->Allocate( memRequirements.size, memRequirements.alignment, buffer.Allocation ) ) )
		{
			TRACE( "Could not allocate memory." );
			throw std::runtime_error( "Could not allocate memory." );
		}

		VERIFY( VK_SUCCESS == vkBindBufferMemory( Context.LogicalDevice, buffer.Buffer, buffer.Allocation.DeviceMemory, buffer.Allocation.Offset ) );
	}
}

void VulkanStagingManager::CreateCommandPool( void )
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_TRANSIENT_BIT | VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = Context.TransferFamilyIndex;

	VERIFY( VK_SUCCESS == vkCreateCommandPool( Context.LogicalDevice, &commandPoolCreateInfo, NULL, &CommandPool ) );
}

void VulkanStagingManager::CreateCommandBuffers( void )
{
	VkCommandBufferAllocateInfo allocInfo = {};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	for( auto& buffer : StagingBuffers )
	{
		vkAllocateCommandBuffers( Context.LogicalDevice, &allocInfo, &( buffer.CommandBuffer ) );
	}
}

void VulkanStagingManager::CleanUp( void )
{
	for( auto& buffer : StagingBuffers )
	{
		vkDestroyBuffer( Context.LogicalDevice, buffer.Buffer, nullptr );
	}

	vkDestroyCommandPool( Context.LogicalDevice, CommandPool, nullptr );

	MemoryPool->CleanUp();
}

void VulkanStagingManager::BeginRecording( void )
{
	VkCommandBufferBeginInfo beginInfo = {};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	//This also resets the command buffer
	VERIFY( VK_SUCCESS == vkBeginCommandBuffer( StagingBuffers[CurrentBufferIndex].CommandBuffer, &beginInfo ) );
}

void VulkanStagingManager::EndRecording( void )
{
	VERIFY( VK_SUCCESS == vkEndCommandBuffer( StagingBuffers[CurrentBufferIndex].CommandBuffer ) );
}

//This needs to happen prior to rendering
void VulkanStagingManager::SubmitQueue( void )
{
	EndRecording();

	VkSubmitInfo submitInfo = {};

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &( StagingBuffers[CurrentBufferIndex].CommandBuffer );

	if( StagingBuffers[CurrentBufferIndex].Submitted )
	{
		Wait( StagingBuffers[CurrentBufferIndex] );
	}

	VERIFY( VK_SUCCESS == vkQueueSubmit( Context.TransferQueue, 1, &submitInfo, StagingBuffers[CurrentBufferIndex].Fence ) );

	StagingBuffers[CurrentBufferIndex].Submitted = true;

	CurrentBufferIndex = ( CurrentBufferIndex + 1 ) % 2;

	if( StagingBuffers[CurrentBufferIndex].Submitted )
	{
		Wait( StagingBuffers[CurrentBufferIndex] );

		StagingBuffers[CurrentBufferIndex].Submitted = false;

		//TODO: Reset the staging buffer
		//StagingBuffers[CurrentBufferIndex].Offset = 0;
	}

	BeginRecording();
}

void VulkanStagingManager::StageData( const void* SourceData, VkDeviceSize Size, VkBuffer Destination )
{
	if( ( StagingBuffers[CurrentBufferIndex].Offset + Size ) > MaxBufferSize )
	{
		//Force Submit now?
		TRACE( "Not enough space in current staging buffer. Forcing submit to queue!" );
		SubmitQueue();
	}

	auto& buffer = StagingBuffers[CurrentBufferIndex];

	memcpy( buffer.Allocation.Data + buffer.Offset, SourceData, (size_t)Size );

	VkBufferCopy copyRegion = {};

	copyRegion.srcOffset = buffer.Offset;
	copyRegion.dstOffset = 0;
	copyRegion.size = Size;
	
	vkCmdCopyBuffer( buffer.CommandBuffer, buffer.Buffer, Destination, 1, &copyRegion );
	
	buffer.Offset += Size;
}

void VulkanStagingManager::Wait( StagingBuffer& Stage )
{
	int timeout = 100;

	auto result = vkWaitForFences( Context.LogicalDevice, 1, &(Stage.Fence), VK_TRUE, timeout );

	if( result == VK_SUCCESS )
	{
		VERIFY( VK_SUCCESS == vkResetFences( Context.LogicalDevice, 1, &(Stage.Fence) ) );
	}
}

// void VulkanStagingManager::Flush( void )
// {

// }