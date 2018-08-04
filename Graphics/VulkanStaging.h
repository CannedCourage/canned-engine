#ifndef _VULKANSTAGING_H_
#define _VULKANSTAGING_H_

#include <vulkan/vulkan.hpp>

#include <vector>
#include <memory>

#include "Graphics/VulkanAllocation.h"

const int DefaultBufferSizeMB = 32;
const int DefaultBufferSize = DefaultBufferSizeMB * 1000000;

struct StagingBuffer
{
	friend class VulkanStagingManager;

public:

	//unsigned char* Data = nullptr;
    
	VkBuffer Buffer = VK_NULL_HANDLE;
	VkDeviceSize Offset = 0;
	
    VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;
	
	vkAllocation Allocation = {};

	VkFence Fence = VK_NULL_HANDLE;
	bool Submitted = false;
};

class GraphicsVK;

class VulkanStagingManager
{
private:
protected:

	GraphicsVK& Context;

	std::unique_ptr<VulkanMemoryPool> MemoryPool;

	VkCommandPool CommandPool = VK_NULL_HANDLE;

	int MaxBufferSize = DefaultBufferSize;
	int CurrentBufferIndex = 0;

	std::vector<StagingBuffer> StagingBuffers;

	// This waits until the command buffer carrying the copy commands is done.
	void Wait( StagingBuffer& Stage );

	void CreateBuffersAndMemory( void );
	void CreateCommandPool( void );
	void CreateCommandBuffers( void );

	void BeginRecording( void );
	void EndRecording( void );
public:

	VulkanStagingManager( GraphicsVK& Context );

	void Init( void );
	void CleanUp( void );

	VkBuffer Stage( const int Size, vkAllocation& Allocation );
	void StageData( const void* SourceData, VkDeviceSize Size, VkBuffer DestinationBuffer );

	void RecordCopyBufferCommand( VkBuffer Source, VkBuffer Destination, VkDeviceSize Size );

	// Flush will drain all data for the current staging buffer.
	void Flush();

	void SubmitQueue( void );
};

#endif //_VULKANSTAGING_H_