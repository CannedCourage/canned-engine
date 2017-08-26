#pragma once

#ifndef _VULKANSTAGING_H_
#define _VULKANSTAGING_H_

#include <vulkan/vulkan.hpp>

#include <vector>

#include "Graphics/VulkanAllocation.h"

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

	//VulkanMemoryPool MemoryPool{ Context, 0,  };

	//unsigned char* MappedData = nullptr;

	//VkDeviceMemory Memory = VK_NULL_HANDLE;

	VkCommandPool CommandPool = VK_NULL_HANDLE;
	VkCommandBuffer CommandBuffer = VK_NULL_HANDLE;

	//int MaxBufferSize = 0;
	//int CurrentBuffer = 0;

	//std::vector<StagingBuffer> Buffers;

	// This waits until the command buffer carrying the copy commands is done.
	//void Wait( StagingBuffer & stage );

	void CreateCommandPool( void );
	void CreateCommandBuffer( void );

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