#ifndef _VULKANSTAGING_H_
#define _VULKANSTAGING_H_

#include <vulkan/vulkan.hpp>

#include <vector>

struct StagingBuffer
{
	unsigned char* Data = nullptr;

	VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
	VkBuffer Buffer = VK_NULL_HANDLE;
	VkFence Fence = VK_NULL_HANDLE;
	VkDeviceSize Offset = 0;
	
	bool submitted = false;
};

class GraphicsVK;

class VulkanStagingManager
{
private:
protected:

	GraphicsVK& Context;

	unsigned char* MappedData = nullptr;

	VkDeviceMemory Memory = VK_NULL_HANDLE;
	VkCommandPool CommandPool = VK_NULL_HANDLE;

	int MaxBufferSize = 0;
	int CurrentBuffer = 0;

	std::vector<StagingBuffer> Buffers;

	// This waits until the command buffer carrying the copy commands is done.
	void Wait( StagingBuffer & stage );
public:

	VulkanStagingManager( GraphicsVK& Context );

	void Init();
	void Shutdown();

	unsigned char* Stage( const int size, const int alignment, VkCommandBuffer & commandBuffer, VkBuffer & buffer, int & bufferOffset );

	// Flush will drain all data for the current staging buffer.
	void Flush();
};

#endif //_VULKANSTAGING_H_