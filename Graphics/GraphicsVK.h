#pragma once

#ifndef _GRAPHICSVK_H_
#define _GRAPHICSVK_H_

#include <vector>
#include <list>

#include <vulkan/vulkan.hpp>
#pragma comment(lib, "vulkan-1.lib")

#include "Logging/Log.h"

#include "Graphics/VulkanAllocation.h"

struct GPU
{
	VkPhysicalDevice Device;
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	std::vector<VkExtensionProperties> ExtensionProperties;
	VkSurfaceCapabilitiesKHR SurfaceCapabilities;
	std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	std::vector<VkPresentModeKHR> PresentModes;
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	VkPhysicalDeviceProperties DeviceProperties;
};

class System;

class GraphicsVK
{
private:

	const bool enableDebugLayers = true; //Get from settings file later

	std::vector<char> ReadFile( const std::string& Filename );
protected:

	Log log{ "GraphicsVK" };

	System& system;

	std::vector<const char *> InstanceExtensions;
	std::vector<const char *> DeviceExtensions;
	std::vector<const char *> ValidationLayers;

	VkInstance Instance;
	VkSurfaceKHR Surface;

	std::vector<GPU> GPUs;

	int GraphicsFamilyIndex = 0;
	int PresentFamilyIndex = 0;
	VkPhysicalDevice PhysicalDevice;

	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	std::vector<VkSemaphore> AcquireSemaphores;
	std::vector<VkSemaphore> RenderCompleteSemaphores;

	VkCommandPool CommandPool;
	std::vector<VkCommandBuffer> CommandBuffers;
	std::vector<VkFence> CommandBufferFences;

	VkSwapchainKHR SwapChain;
	VkFormat SwapChainFormat;
	VkPresentModeKHR SwapChainPresentMode;
	VkExtent2D SwapChainExtent;

	std::vector<VkImageView> SwapChainImageViews;

	VkFormat DepthFormat;
	VkImage DepthBuffer;
	vkAllocation DepthBufferMemory;
	VkImageView DepthBufferView;

	VkRenderPass RenderPass;

	std::vector<VkFramebuffer> FrameBuffers;

	VulkanAllocator MemoryAllocator{ *this };

	VkShaderModule vertShaderModule;
	VkShaderModule fragShaderModule;

	VkPipelineLayout PipelineLayout;
	VkPipeline GraphicsPipeline;

	void ReadSettings( void );
	void WriteSettings( void );

	//Vulkan
	void CreateInstance( void );
	void CreateSurface( void );
	void EnumeratePhysicalDevices( void );
	void SelectPhysicalDevice( void );
	void CreateLogicalDeviceAndQueues( void );
	void CreateSemaphores( void );
	void CreateCommandPool( void );
	void CreateCommandBuffer( void );
	void CreateSwapChain( void );
	void CreateRenderTargets( void );
	void CreateRenderPass( void );
	void CreatePipeline( void );
	void CreateFrameBuffers( void );
	void RecordCommands( void );

	VkSurfaceFormatKHR ChooseSurfaceFormat( void );
	VkPresentModeKHR ChoosePresentMode( void );
	VkExtent2D ChooseSurfaceExtent( void );
	VkFormat ChooseSupportedFormat( const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features );
public:

	const unsigned int bufferCount = 2;
	GPU* GPUInfo = nullptr;

	VkDevice LogicalDevice;

	GraphicsVK( System& s );

	void Init( void );
	void CleanUp( void );

	void DrawFrame( void );
};

#endif //_GRAPHICSVK_H_