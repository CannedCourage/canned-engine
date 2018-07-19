#ifndef _VULKANSTAGING_H_
#define _VULKANSTAGING_H_

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Graphics/VulkanAllocation.h"

const int DefaultBufferSizeMB = 64;
const int DefaultBufferSize = DefaultBufferSizeMB * 1000000;

struct StagingBuffer
{
	friend class VulkanStagingManager;

public:
	unsigned char* Data = nullptr;

	VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
	VkBuffer Buffer = VK_NULL_HANDLE;
	vkAllocation Allocation;

	unsigned int BufferIndex;

	StagingBuffer( unsigned int Index ) : BufferIndex( Index )
	{
	}
private:

	VkFence Fence = VK_NULL_HANDLE;
	bool submitted = false;
};

class GraphicsVK;

class VulkanStagingManager
{
private:
protected:

	GraphicsVK& Context;

	VkCommandPool CommandPool = VK_NULL_HANDLE;

	//int MaxBufferSize = 0;
	int CurrentBufferIndex = 0;

	std::vector<StagingBuffer> Buffers;

	// This waits until the command buffer carrying the copy commands is done.
	//void Wait( StagingBuffer & stage );

	void CreateCommandPool( void );
	void CreateCommandBuffer( StagingBuffer& CurrentBuffer );
	void CreateBuffer( StagingBuffer& CurrentBuffer );

	void BeginRecording( void );
	void EndRecording( void );
public:

	VulkanStagingManager( GraphicsVK& Context );

	void Init( void );
	void CleanUp( void );

	VkBuffer Stage( const int Size, vkAllocation& Allocation );

	void CopyBuffer( VkBuffer Source, VkBuffer Destination, VkDeviceSize Size );

	// Flush will drain all data for the current staging buffer.
	//void Flush();

	void SubmitQueue( void );
};

#endif //_VULKANSTAGING_H_