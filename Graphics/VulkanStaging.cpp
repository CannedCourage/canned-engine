#include "Graphics/VulkanStaging.h"
#include "Graphics/GraphicsVK.h"

VulkanStagingManager::VulkanStagingManager( GraphicsVK& Context ) : Context( Context ) //Buffers{ Context.BufferCount }
{
}

void VulkanStagingManager::Init( void )
{
	CreateCommandPool();

	CreateCommandBuffer();

	//Instantiate Buffer
	//Use Buffer mem requirements to instantiate VulkanMemoryPool?

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

void VulkanStagingManager::CreateCommandBuffer( void )
{
	VkCommandBufferAllocateInfo allocInfo = {};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	vkAllocateCommandBuffers( Context.LogicalDevice, &allocInfo, &CommandBuffer );
}

void VulkanStagingManager::BeginRecording( void )
{
	VkCommandBufferBeginInfo beginInfo = {};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VERIFY( VK_SUCCESS == vkBeginCommandBuffer( CommandBuffer, &beginInfo ) );
}

void VulkanStagingManager::EndRecording( void )
{
	VERIFY( VK_SUCCESS == vkEndCommandBuffer( CommandBuffer ) );
}

void VulkanStagingManager::CleanUp( void )
{
	vkFreeCommandBuffers( Context.LogicalDevice, CommandPool, 1, &CommandBuffer );

	vkDestroyCommandPool( Context.LogicalDevice, CommandPool, nullptr );
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
	
	vkCmdCopyBuffer( CommandBuffer, Source, Destination, 1, &copyRegion );
}

void VulkanStagingManager::SubmitQueue( void )
{
	EndRecording();

	VERIFY( VK_SUCCESS == vkQueueWaitIdle( Context.TransferQueue ) );

	VkSubmitInfo submitInfo = {};

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffer;

	VERIFY( VK_SUCCESS == vkQueueSubmit( Context.TransferQueue, 1, &submitInfo, VK_NULL_HANDLE ) );

	VERIFY( VK_SUCCESS == vkQueueWaitIdle( Context.TransferQueue ) );

	//Clean up temp "staging" buffer

	//OR

	//Do separate check for VkEvent?

	//Reset command buffer?
	BeginRecording();
}