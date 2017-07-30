#ifndef _GRAPHICSVK_H_
#define _GRAPHICSVK_H_

#define VK_USE_PLATFORM_WIN32_KHR

#include <vector>
#include <list>
#include <vulkan/vulkan.hpp>

#pragma comment(lib, "vulkan-1.lib")

#include "Logging/Log.h"

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
class WindowMS;

class GraphicsVK
{
private:

	const bool enableDebugLayers = true; //Get from settings file later
protected:

	Log log;

	System& system;
	WindowMS& window;

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

	std::vector<VkSemaphore> AcquireSemaphores{ bufferCount };
	std::vector<VkSemaphore> RenderCompleteSemaphores{ bufferCount };

	VkCommandPool CommandPool;
	std::vector<VkCommandBuffer> CommandBuffer{ bufferCount };
	std::vector<VkFence> CommandBufferFences{ bufferCount };

	VkSwapchainKHR SwapChain;
	VkFormat SwapChainFormat;
	VkPresentModeKHR SwapChainPresentMode;
	VkExtent2D SwapChainExtent;

	std::vector<VkImageView> SwapChainImageViews;

	VkFormat DepthFormat;
	VkImage DepthBuffer;
	VkImageView DepthBufferView;

	VkRenderPass RenderPass;

	std::vector<VkFramebuffer> FrameBuffers{ bufferCount };

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
	void CreateFrameBuffers( void );

	VkSurfaceFormatKHR ChooseSurfaceFormat( void );
	VkPresentModeKHR ChoosePresentMode( void );
	VkExtent2D ChooseSurfaceExtent( void );
	VkFormat ChooseSupportedFormat( const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features );
public:

	const unsigned int bufferCount = 2;
	GPU* GPUInfo = nullptr;

	VkDevice LogicalDevice;

	GraphicsVK( System& s );
	~GraphicsVK( void );

	void Init( void );
	void CleanUp( void );
};

#endif //_GRAPHICSVK_H_