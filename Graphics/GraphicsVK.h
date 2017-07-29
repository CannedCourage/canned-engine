#ifndef _GRAPHICSVK_H_
#define _GRAPHICSVK_H_

#define VK_USE_PLATFORM_WIN32_KHR

struct GPU
{
	VkPhysicalDevice Device;
	std::vector<VkQueueFamilyProperties> QueueFamilyProperties;
	std::vector<VkExtensionProperties> ExtensionProperties;
	VkSurfaceCapabilitiesKHR SurfaceCapabilities = nullptr;
	std::vector<VkSurfaceFormatKHR> SurfaceFormats;
	std::vector<VkPresentModeKHR> PresentModes;
	VkPhysicalDeviceMemoryProperties MemoryProperties;
	VkPhysicalDeviceProperties DeviceProperties;
};

class GraphicsVK
{
protected:

	Log log;

	System& system;
	Settings& settings;
	iWindow& window;

	std::vector<const char *> InstanceExtensions;
	std::vector<const char *> DeviceExtensions;
	std::vector<const char *> ValidationLayers;

	VkInstance Instance;
	VkSurfaceKHR Surface;

	std::vector<GPU> GPUs;

	int GraphicsFamilyIndex = 0;
	int PresentFamilyIndex = 0;
	VkPhysicalDevice PhysicalDevice;
	GPU* GPUInfo = nullptr;

	VkDevice LogicalDevice;

	VkQueue GraphicsQueue;
	VkQueue PresentQueue;

	const unsigned int bufferCount = 2;
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

	std::vector<VkFramebuffer> FrameBuffers{bufferCount};

	void ReadSettings( void );
	void WriteSettings( void );

	//Vulkan
	void CreateInstance( void );
	void CreateSurface( void );
public:

	GraphicsVK( System& s );
	~GraphicsVK( void );

	void Init( void );
	void CleanUp( void );
};

#endif //_GRAPHICSVK_H_