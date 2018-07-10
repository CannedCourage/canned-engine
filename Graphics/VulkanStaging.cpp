#include "Graphics/VulkanStaging.h"
#include "Graphics/GraphicsVK.h"

#include <algorithm>

VulkanStagingManager::VulkanStagingManager( GraphicsVK& Context ) : Context( Context ) //Buffers{ Context.BufferCount }
{
}

void VulkanStagingManager::Init( void )
{
	CreateCommandPool();

	for( int ii = 0; ii < Context.BufferCount; ii++ )
	{
		StagingBuffer buffer(ii);

		CreateCommandBuffer( buffer );

		CreateBuffer( buffer );

		Buffers.push_back( buffer );
	}

	BeginRecording();
}

void VulkanStagingManager::CreateCommandPool( void )
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = Context.TransferFamilyIndex;

	VERIFY( VK_SUCCESS == vkCreateCommandPool( Context.LogicalDevice, &commandPoolCreateInfo, NULL, &CommandPool ) );
}

void VulkanStagingManager::CreateCommandBuffer( StagingBuffer& CurrentBuffer )
{
	VkCommandBufferAllocateInfo allocInfo = {};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers( Context.LogicalDevice, &allocInfo, &CurrentBuffer.CommandBuffer );
}

void VulkanStagingManager::CreateBuffer( StagingBuffer& CurrentBuffer )
{
	VkBufferCreateInfo bufferInfo = {};
	bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
	bufferInfo.size = DefaultBufferSize;
	bufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
	bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VERIFY( VK_SUCCESS == vkCreateBuffer( Context.LogicalDevice, &bufferInfo, nullptr, &( CurrentBuffer.Buffer ) ) );

	VkMemoryRequirements memRequirements;
	vkGetBufferMemoryRequirements( Context.LogicalDevice, CurrentBuffer.Buffer, &memRequirements );

	CurrentBuffer.Allocation = Context.MemoryAllocator.Allocate(
		memRequirements.size,
		memRequirements.alignment,
		memRequirements.memoryTypeBits,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		0
	);

	VERIFY( VK_SUCCESS == vkBindBufferMemory( Context.LogicalDevice, CurrentBuffer.Buffer, CurrentBuffer.Allocation.DeviceMemory, CurrentBuffer.Allocation.Offset ) );
}

void VulkanStagingManager::CleanUp( void )
{
	for( StagingBuffer& buffer : Buffers )
	{
		vkFreeCommandBuffers( Context.LogicalDevice, CommandPool, 1, &( buffer.CommandBuffer ) );
	}

	vkDestroyCommandPool( Context.LogicalDevice, CommandPool, nullptr );
}

void VulkanStagingManager::BeginRecording( void )
{
	VkCommandBufferBeginInfo beginInfo = {};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VERIFY( VK_SUCCESS == vkBeginCommandBuffer( Buffers[CurrentBufferIndex].CommandBuffer, &beginInfo ) );
}

void VulkanStagingManager::EndRecording( void )
{
	VERIFY( VK_SUCCESS == vkEndCommandBuffer( Buffers[CurrentBufferIndex].CommandBuffer ) );
}

void VulkanStagingManager::SubmitQueue( void )
{
	EndRecording();

	VERIFY( VK_SUCCESS == vkQueueWaitIdle( Context.TransferQueue ) );

	VkSubmitInfo submitInfo = {};

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &( Buffers[CurrentBufferIndex].CommandBuffer );

	VERIFY( VK_SUCCESS == vkQueueSubmit( Context.TransferQueue, 1, &submitInfo, VK_NULL_HANDLE ) );

	VERIFY( VK_SUCCESS == vkQueueWaitIdle( Context.TransferQueue ) );

	//Clean up temp "staging" buffer

	//OR

	//Do separate check for VkEvent?

	CurrentBufferIndex = ( CurrentBufferIndex + 1 ) % 2;

	//Reset command buffer?
	BeginRecording();
}

VkBuffer VulkanStagingManager::Stage( const int Size, vkAllocation& Allocation )
{
	//TODO: StagingManager needs to own the buffer and memory
	VkBuffer stagingBuffer;

	stagingBuffer = Context.CreateBuffer(
		Size,
		VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
		Allocation
		);

	return stagingBuffer;
}

void VulkanStagingManager::CopyBuffer( VkBuffer Source, VkBuffer Destination, VkDeviceSize Size )
{
	VkBufferCopy copyRegion = {};

	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = Size;
	
	vkCmdCopyBuffer( Buffers[CurrentBufferIndex].CommandBuffer, Source, Destination, 1, &copyRegion );
}