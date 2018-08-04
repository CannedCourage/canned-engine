#ifndef NOMINMAX
#define NOMINMAX
#endif

#include <unordered_set>
#include <fstream>
#include <limits>
#include <bitset>
#include <iterator>
#include <algorithm>
#include <set>

#include "Graphics/GraphicsVK.h"

#include <GLFW/glfw3.h>

#define settingsFile "w:/engine/data/settings/MainSettings.json"

static_assert( sizeof(unsigned int) == 4, "Vulkan relies on 32-bit integer data type" );

std::vector<Vertex> vertices =
{
	{{-0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}},
    {{0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}},
    {{0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f}, {1.0f, 1.0f, 1.0f}}
};

std::vector<uint16_t> drawIndices = { 0, 1, 2, 2, 3, 0 };

GraphicsVK::GraphicsVK( void )
{
	AcquireSemaphores.resize( BufferCount );
	RenderCompleteSemaphores.resize( BufferCount );
	FrameBuffers.resize( BufferCount );

	InstanceExtensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
	
	DeviceExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );

	if( enableDebugLayers )
	{
		InstanceExtensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );

		ValidationLayers.push_back( "VK_LAYER_LUNARG_standard_validation" );

		if( !CheckValidationLayerSupport() )
		{
			throw std::runtime_error( "Validation layers requested, but not available!" );
		}
	}
}

bool GraphicsVK::CheckValidationLayerSupport( void )
{
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties( &layerCount, nullptr );

	std::vector<VkLayerProperties> availableLayers( layerCount );
	vkEnumerateInstanceLayerProperties( &layerCount, availableLayers.data() );

	//std::sort( ValidationLayers.begin(), ValidationLayers.end() );
	//std::sort( availableLayers.begin(), availableLayers.end() );

	//bool includes = std::includes( availableLayers.begin(), availableLayers.end(), ValidationLayers.begin(), ValidationLayers.end() );

	//TODO: Review this
	for( const char* layer : ValidationLayers )
	{
		bool layerFound = false;

		for( const auto& layerProperties : availableLayers )
		{
			if( strcmp( layer, layerProperties.layerName ) == 0)
			{
				layerFound = true;
				break;
			}
		}

		if( !layerFound )
		{
			return false;
		}
	}

	return true;
}

VKAPI_ATTR VkBool32 VKAPI_CALL GraphicsVK::DebugCallback( VkDebugReportFlagsEXT flags, VkDebugReportObjectTypeEXT objType, uint64_t obj, size_t location, int32_t code, const char* layerPrefix, const char* msg, void* userData )
{
    //std::cerr << "Validation layer: " << msg << std::endl;
    TRACE( msg );

    return VK_FALSE;
}

void GraphicsVK::CreateDebugReportCallback( void )
{
	VkDebugReportCallbackCreateInfoEXT createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT;
	createInfo.flags = VK_DEBUG_REPORT_ERROR_BIT_EXT | VK_DEBUG_REPORT_WARNING_BIT_EXT;
	createInfo.pfnCallback = DebugCallback;

	if( CreateDebugReportCallbackEXT( Instance, &createInfo, nullptr, &Callback ) != VK_SUCCESS )
	{
		throw std::runtime_error( "failed to set up debug callback!" );
	}
}

VkResult CreateDebugReportCallbackEXT( VkInstance instance, const VkDebugReportCallbackCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugReportCallbackEXT* pCallback )
{
	auto func = (PFN_vkCreateDebugReportCallbackEXT) vkGetInstanceProcAddr( instance, "vkCreateDebugReportCallbackEXT" );

	if( func != nullptr )
	{
		return func( instance, pCreateInfo, pAllocator, pCallback );
	}
	else
	{
		return VK_ERROR_EXTENSION_NOT_PRESENT;
	}
}

void DestroyDebugReportCallbackEXT( VkInstance instance, VkDebugReportCallbackEXT callback, const VkAllocationCallbacks* pAllocator )
{
	auto func = (PFN_vkDestroyDebugReportCallbackEXT) vkGetInstanceProcAddr( instance, "vkDestroyDebugReportCallbackEXT" );

	if( func != nullptr )
	{
		func( instance, callback, pAllocator );
	}
}

void GraphicsVK::GetInstanceExtensions( void )
{
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions;

	glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
	ASSERT( glfwExtensionCount > 0 );
	ASSERT( glfwExtensions != NULL );

	InstanceExtensions.insert( InstanceExtensions.end(), glfwExtensions, glfwExtensions + glfwExtensionCount );
}

void GraphicsVK::Init( void )
{
	TRACE( "GraphicsVK Init" );

	ReadSettings();

	GetInstanceExtensions();

	CreateInstance();

	CreateSurface();

	EnumeratePhysicalDevices();

	SelectPhysicalDevice();

	CreateLogicalDeviceAndQueues();

	CreateSemaphores();

	CreateCommandPool();

	MemoryAllocator.Init();

	StagingManager.Init();

	InitSwapChain();

	CreateVertexBuffer();

	CreateIndexBuffer();

	RecordCommands();
}

void GraphicsVK::InitSwapChain( void )
{
	CreateCommandBuffer();

	CreateSwapChain();

	CreateRenderTargets();

	CreateRenderPass();

	CreatePipeline();

	CreateFrameBuffers();
}

void GraphicsVK::CleanUpSwapChain( void )
{
	for( VkFramebuffer& frame : FrameBuffers )
	{
		vkDestroyFramebuffer( LogicalDevice, frame, nullptr );
	}

	vkFreeCommandBuffers( LogicalDevice, CommandPool, CommandBuffers.size(), CommandBuffers.data() );

	vkDestroyPipeline( LogicalDevice, GraphicsPipeline, nullptr );
	vkDestroyPipelineLayout( LogicalDevice, PipelineLayout, nullptr );

	vkDestroyShaderModule( LogicalDevice, fragShaderModule, nullptr );
	vkDestroyShaderModule( LogicalDevice, vertShaderModule, nullptr );

	vkDestroyRenderPass( LogicalDevice, RenderPass, nullptr );

	vkDestroyImageView( LogicalDevice, DepthBufferView, nullptr );
	MemoryAllocator.Free( DepthBufferMemory );
	vkDestroyImage( LogicalDevice, DepthBuffer, nullptr );

	for( VkImageView& view : SwapChainImageViews )
	{
		vkDestroyImageView( LogicalDevice, view, nullptr );
	}

	vkDestroySwapchainKHR( LogicalDevice, SwapChain, nullptr );
}

void GraphicsVK::RecreateSwapChain( void )
{
	vkDeviceWaitIdle( LogicalDevice );

	CleanUpSwapChain();

	InitSwapChain();
}

void GraphicsVK::CleanUp( void )
{
	vkDeviceWaitIdle( LogicalDevice );

	CleanUpSwapChain();

	vkDestroyBuffer( LogicalDevice, VertexBuffer, nullptr );

    vkDestroyBuffer( LogicalDevice, IndexBuffer, nullptr );

	MemoryAllocator.CleanUp();

	for( VkFence& fence : CommandBufferFences )
	{
		vkDestroyFence( LogicalDevice, fence, nullptr );
	}

	vkDestroyCommandPool( LogicalDevice, CommandPool, nullptr );

	for( VkSemaphore& semaphore : RenderCompleteSemaphores )
	{
		vkDestroySemaphore( LogicalDevice, semaphore, nullptr );
	}

	for( VkSemaphore& semaphore : AcquireSemaphores )
	{
		vkDestroySemaphore( LogicalDevice, semaphore, nullptr );
	}

	vkDestroyDevice( LogicalDevice, nullptr );
	vkDestroySurfaceKHR( Instance, Surface, nullptr );

	if( enableDebugLayers )
	{
		DestroyDebugReportCallbackEXT( Instance, Callback, nullptr );
	}

	vkDestroyInstance( Instance, nullptr );

	WriteSettings();
}

void GraphicsVK::CreateInstance( void )
{
	VkApplicationInfo appInfo = {};

	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "VkTestApp";
	appInfo.applicationVersion = 1;
	appInfo.pEngineName = "Canned-Engine";
	appInfo.engineVersion = 1;
	appInfo.apiVersion = VK_MAKE_VERSION( 1, 0, VK_HEADER_VERSION );

	VkInstanceCreateInfo createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
	createInfo.enabledExtensionCount = InstanceExtensions.size();
	createInfo.ppEnabledExtensionNames = InstanceExtensions.data();
	createInfo.enabledLayerCount = ValidationLayers.size();
	createInfo.ppEnabledLayerNames = ValidationLayers.data();

	VERIFY( VK_SUCCESS == vkCreateInstance( &createInfo, NULL, &Instance ) );

	if( enableDebugLayers )
	{
		CreateDebugReportCallback();
	}
}

void GraphicsVK::CreateSurface( void )
{
	VERIFY( VK_SUCCESS == glfwCreateWindowSurface( Instance, Window, nullptr, &Surface ) );
	ASSERT( Surface != nullptr );
}

void GraphicsVK::EnumeratePhysicalDevices( void )
{
	unsigned int numDevices = 0;
	std::vector<VkPhysicalDevice> devices;

	VERIFY( VK_SUCCESS == vkEnumeratePhysicalDevices( Instance, &numDevices, NULL ) );
	ASSERT( numDevices > 0 );

	devices.resize( numDevices );

	VERIFY( VK_SUCCESS == vkEnumeratePhysicalDevices( Instance, &numDevices, devices.data() ) );
	ASSERT( numDevices > 0 );

	GPUs.resize( numDevices );

	for( int ii = 0; ii < numDevices; ii++ )
	{
		GPU& gpu = GPUs[ii];
		gpu.Device = devices[ii];

		{
			unsigned int numQueues = 0;

			vkGetPhysicalDeviceQueueFamilyProperties( gpu.Device, &numQueues, NULL );
			ASSERT( numQueues > 0 );

			gpu.QueueFamilyProperties.resize( numQueues );

			vkGetPhysicalDeviceQueueFamilyProperties( gpu.Device, &numQueues, gpu.QueueFamilyProperties.data() );
			ASSERT( numQueues > 0 );
		}

		{
			unsigned int numExtensions = 0;

			VERIFY( VK_SUCCESS == vkEnumerateDeviceExtensionProperties( gpu.Device, NULL, &numExtensions, NULL ) );
			ASSERT( numExtensions > 0 );

			gpu.ExtensionProperties.resize( numExtensions );

			VERIFY( VK_SUCCESS == vkEnumerateDeviceExtensionProperties( gpu.Device, NULL, &numExtensions, gpu.ExtensionProperties.data() ) );
			ASSERT( numExtensions > 0 );
		}

		VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceCapabilitiesKHR( gpu.Device, Surface, &(gpu.SurfaceCapabilities) ) );

		{
			unsigned int numFormats = 0;

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR( gpu.Device, Surface, &numFormats, NULL ) );
			ASSERT( numFormats > 0 );

			gpu.SurfaceFormats.resize( numFormats );

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR( gpu.Device, Surface, &numFormats, gpu.SurfaceFormats.data() ) );
			ASSERT( numFormats > 0 );
		}

		{
			unsigned int numPresentModes = 0;

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR( gpu.Device, Surface, &numPresentModes, NULL ) );
			ASSERT( numPresentModes > 0 );

			gpu.PresentModes.resize( numPresentModes );

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR( gpu.Device, Surface, &numPresentModes, gpu.PresentModes.data() ) );
			ASSERT( numPresentModes > 0 );
		}

		vkGetPhysicalDeviceMemoryProperties( gpu.Device, &(gpu.MemoryProperties) );

		vkGetPhysicalDeviceProperties( gpu.Device, &(gpu.DeviceProperties) );
	}
}

bool GraphicsVK::CheckPhysicalDeviceExtensionSupport( GPU Device )
{
	//TODO: Review this
	std::set<std::string> requiredExtensions( DeviceExtensions.begin(), DeviceExtensions.end() );

	for( const auto& extension : Device.ExtensionProperties )
	{
		requiredExtensions.erase( extension.extensionName );
	}

	return requiredExtensions.empty();
}

void GraphicsVK::SelectPhysicalDevice( void )
{
	for( GPU& gpu : GPUs )
	{
		int graphicsIdx = -1;
		int presentIdx = -1;
		int transferIdx = -1;

		if( !CheckPhysicalDeviceExtensionSupport( gpu ) )
		{
			continue;
		}

		if( gpu.SurfaceFormats.size() == 0 ){ continue; }

		if( gpu.PresentModes.size() == 0 ){ continue; }

		for( int ii = 0; ii < gpu.QueueFamilyProperties.size(); ii++ )
		{
			const VkQueueFamilyProperties& props = gpu.QueueFamilyProperties[ii];

			if( props.queueCount == 0 ){ continue; }

			if( props.queueFlags & VK_QUEUE_GRAPHICS_BIT )
			{
				graphicsIdx = ii;

				break;
			}
		}

		for( int ii = 0; ii < gpu.QueueFamilyProperties.size(); ii++ )
		{
			const VkQueueFamilyProperties& props = gpu.QueueFamilyProperties[ii];

			if( props.queueCount == 0 ){ continue; }

			VkBool32 supportsPresent = VK_FALSE;

			vkGetPhysicalDeviceSurfaceSupportKHR( gpu.Device, ii, Surface, &supportsPresent );

			if( supportsPresent )
			{
				presentIdx = ii;
				
				break;
			}
		}

		for( int ii = 0; ii < gpu.QueueFamilyProperties.size(); ii++ )
		{
			const VkQueueFamilyProperties& props = gpu.QueueFamilyProperties[ii];

			if( props.queueCount == 0 ){ continue; }

			if( !( props.queueFlags & VK_QUEUE_GRAPHICS_BIT ) && ( props.queueFlags & VK_QUEUE_TRANSFER_BIT ) )
			{
				transferIdx = ii;

				break;
			}
		}

		if( graphicsIdx >= 0 && presentIdx >= 0 && transferIdx >= 0 )
		{
			GraphicsFamilyIndex = graphicsIdx;
			PresentFamilyIndex = presentIdx;
			TransferFamilyIndex = transferIdx;
			PhysicalDevice = gpu.Device;
			GPUInfo = &gpu;

			return;
		}
	}

	throw new std::runtime_error( "Could not find a physical device which fits our desired profile" );
}

void GraphicsVK::CreateLogicalDeviceAndQueues( void )
{
	std::unordered_set<int> uniqueIndex;

	uniqueIndex.insert( GraphicsFamilyIndex );
	uniqueIndex.insert( PresentFamilyIndex );
	uniqueIndex.insert( TransferFamilyIndex );

	std::vector<VkDeviceQueueCreateInfo> deviceQueueInfo;

	const float priority = 1.0f;

	for( int ii : uniqueIndex )
	{
		VkDeviceQueueCreateInfo qInfo = {};

		qInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
		qInfo.queueFamilyIndex = ii;
		qInfo.queueCount = 1;

		qInfo.pQueuePriorities = &priority;

		deviceQueueInfo.push_back( qInfo );
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	deviceFeatures.textureCompressionBC = VK_TRUE;
	deviceFeatures.imageCubeArray = VK_TRUE;
	deviceFeatures.depthClamp = VK_TRUE;
	deviceFeatures.depthBiasClamp = VK_TRUE;
	deviceFeatures.depthBounds = VK_TRUE;
	deviceFeatures.fillModeNonSolid = VK_TRUE;

	VkDeviceCreateInfo info = {};

	info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	info.queueCreateInfoCount = deviceQueueInfo.size();
	info.pQueueCreateInfos = deviceQueueInfo.data();
	info.pEnabledFeatures = &deviceFeatures;
	info.enabledExtensionCount = DeviceExtensions.size();
	info.ppEnabledExtensionNames = DeviceExtensions.data();

	if( enableDebugLayers )
	{
		info.enabledLayerCount = ValidationLayers.size();
		info.ppEnabledLayerNames = ValidationLayers.data();
	}
	else
	{
		info.enabledLayerCount = 0;
	}

	// uint32_t count = 0;
	// std::vector<VkExtensionProperties> extProps;

	// vkEnumerateDeviceExtensionProperties( PhysicalDevice, nullptr, &count, nullptr );

	// extProps.resize( count );

	// vkEnumerateDeviceExtensionProperties( PhysicalDevice, nullptr, &count, extProps.data() );	

	// for( VkExtensionProperties& p : extProps )
	// {
	// 	TRACE( p.extensionName );
	// }
	
	VERIFY( VK_SUCCESS == vkCreateDevice( PhysicalDevice, &info, NULL, &LogicalDevice ) );

	vkGetDeviceQueue( LogicalDevice, GraphicsFamilyIndex, 0, &GraphicsQueue );
	vkGetDeviceQueue( LogicalDevice, PresentFamilyIndex, 0, &PresentQueue );
	vkGetDeviceQueue( LogicalDevice, TransferFamilyIndex, 0, &TransferQueue );
}

void GraphicsVK::CreateSemaphores( void )
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};

	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for( int ii = 0; ii < BufferCount; ii++ )
	{
		VERIFY( VK_SUCCESS == vkCreateSemaphore( LogicalDevice, &semaphoreCreateInfo, NULL, &AcquireSemaphores[ii] ) );
		VERIFY( VK_SUCCESS == vkCreateSemaphore( LogicalDevice, &semaphoreCreateInfo, NULL, &RenderCompleteSemaphores[ii] ) );
	}
}

void GraphicsVK::CreateCommandPool( void )
{
	VkCommandPoolCreateInfo commandPoolCreateInfo = {};
	commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;

	commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	commandPoolCreateInfo.queueFamilyIndex = GraphicsFamilyIndex;

	VERIFY( VK_SUCCESS == vkCreateCommandPool( LogicalDevice, &commandPoolCreateInfo, NULL, &CommandPool ) );
}

void GraphicsVK::CreateCommandBuffer( void )
{
	VkCommandBufferAllocateInfo commandBufferAllocateInfo = {};

	commandBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;

	commandBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	commandBufferAllocateInfo.commandPool = CommandPool;
	commandBufferAllocateInfo.commandBufferCount = BufferCount;

	CommandBuffers.resize( BufferCount );
	CommandBufferFences.resize( BufferCount );

	VERIFY( VK_SUCCESS == vkAllocateCommandBuffers( LogicalDevice, &commandBufferAllocateInfo, CommandBuffers.data() ) );

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	for( VkFence& fence : CommandBufferFences )
	{
		VERIFY( VK_SUCCESS == vkCreateFence( LogicalDevice, &fenceCreateInfo, NULL, &fence ) );
	}
}

VkSurfaceFormatKHR GraphicsVK::ChooseSurfaceFormat( void )
{
	if( ( GPUInfo->SurfaceFormats.size() == 1 ) && ( GPUInfo->SurfaceFormats[0].format == VK_FORMAT_UNDEFINED ) )
	{
		VkSurfaceFormatKHR result;
		
		result.format = VK_FORMAT_B8G8R8A8_UNORM;
		result.colorSpace = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR;

		return result;
	}

	for( const VkSurfaceFormatKHR& format : GPUInfo->SurfaceFormats )
	{
		if( ( format.format == VK_FORMAT_B8G8R8A8_UNORM ) && ( format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR ) )
		{
			return format;
		}
	}

	TRACE( "No suitable format available!" );
	return GPUInfo->SurfaceFormats[0];
}

VkPresentModeKHR GraphicsVK::ChoosePresentMode( void )
{
	const VkPresentModeKHR desiredMode = VK_PRESENT_MODE_MAILBOX_KHR;

	for( const VkPresentModeKHR& mode : GPUInfo->PresentModes )
	{
		if( mode == desiredMode )
		{
			return desiredMode;
		}
	}

	//This is always available
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D GraphicsVK::ChooseSurfaceExtent( void )
{
	VkExtent2D extent;

	if( GPUInfo->SurfaceCapabilities.currentExtent.width == -1 )
	{
		extent.width = Settings["display"]["xResolution"];
		extent.height = Settings["display"]["yResolution"];
	}
	else
	{
		extent = GPUInfo->SurfaceCapabilities.currentExtent;
	}

	return extent;
}

void GraphicsVK::CreateSwapChain( void )
{
	VkSurfaceFormatKHR surfaceFormat = ChooseSurfaceFormat();
	VkPresentModeKHR presentMode = ChoosePresentMode();
	VkExtent2D extent = ChooseSurfaceExtent();

	VkSwapchainCreateInfoKHR swapChainCreateInfo = {};

	swapChainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapChainCreateInfo.surface = Surface;

	swapChainCreateInfo.minImageCount = BufferCount;

	swapChainCreateInfo.imageFormat = surfaceFormat.format;
	swapChainCreateInfo.imageColorSpace = surfaceFormat.colorSpace;
	swapChainCreateInfo.imageExtent = extent;
	swapChainCreateInfo.imageArrayLayers = 1;

	swapChainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_SRC_BIT;

	if( GraphicsFamilyIndex != PresentFamilyIndex )
	{
		unsigned int indices[] =
		{
			(unsigned int)GraphicsFamilyIndex,
			(unsigned int)PresentFamilyIndex
		};

		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = indices;
	}
	else
	{
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	swapChainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapChainCreateInfo.presentMode = presentMode;

	swapChainCreateInfo.clipped = VK_TRUE;

	VERIFY( VK_SUCCESS == vkCreateSwapchainKHR( LogicalDevice, &swapChainCreateInfo, NULL, &SwapChain ) );
	SwapChainFormat = surfaceFormat.format;
	SwapChainPresentMode = presentMode;
	SwapChainExtent = extent;

	unsigned int numImages = 0;
	std::vector<VkImage> swapChainImages;

	VERIFY( VK_SUCCESS == vkGetSwapchainImagesKHR( LogicalDevice, SwapChain, &numImages, NULL ) );
	ASSERT( numImages > 0 );

	swapChainImages.resize( numImages );

	VERIFY( VK_SUCCESS == vkGetSwapchainImagesKHR( LogicalDevice, SwapChain, &numImages, swapChainImages.data() ) );
	ASSERT( numImages > 0 );

	for( VkImage& image : swapChainImages )
	{
		VkImageViewCreateInfo imageViewCreateInfo = {};

		imageViewCreateInfo.sType =VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = image;
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = SwapChainFormat;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R; //VK_COMPONENT_SWIZZLE_IDENTITY?
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;
		imageViewCreateInfo.flags = 0;

		VkImageView imageView;
		VERIFY( VK_SUCCESS == vkCreateImageView( LogicalDevice, &imageViewCreateInfo, NULL, &imageView ) );

		SwapChainImageViews.push_back( imageView );
	}
}

VkFormat GraphicsVK::ChooseSupportedFormat( const std::vector<VkFormat>& formats, VkImageTiling tiling, VkFormatFeatureFlags features )
{
	for( const VkFormat& format : formats )
	{
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties( PhysicalDevice, format, &props );

		if( ( tiling == VK_IMAGE_TILING_LINEAR ) && ( ( props.linearTilingFeatures & features ) == features ) )
		{
			return format;
		}
		else
		{
			if( ( tiling == VK_IMAGE_TILING_OPTIMAL  ) && ( ( props.optimalTilingFeatures & features ) == features ) )
			{
				return format;
			}
		}
	}

	TRACE( "Failed to find a supported format." );
	return VK_FORMAT_UNDEFINED;
}

void GraphicsVK::CreateRenderTargets( void )
{
	std::vector<VkFormat> formats{ VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
	VkImageTiling tiling = VK_IMAGE_TILING_OPTIMAL;
	VkFormatFeatureFlags features = VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT;

	DepthFormat = ChooseSupportedFormat( formats, tiling, features );

	VkImageCreateInfo depthImageCreateInfo = {};

	depthImageCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
	depthImageCreateInfo.imageType = VK_IMAGE_TYPE_2D;
	depthImageCreateInfo.extent.width = SwapChainExtent.width;
	depthImageCreateInfo.extent.height = SwapChainExtent.height;
	depthImageCreateInfo.extent.depth = 1;
	depthImageCreateInfo.mipLevels = 1;
	depthImageCreateInfo.arrayLayers = 1;
	depthImageCreateInfo.format = DepthFormat;
	depthImageCreateInfo.tiling = tiling;
	depthImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //VK_IMAGE_LAYOUT_PREINITIALIZED; ???
	depthImageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	
	//Multisample???
	depthImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;

	//Don't know if this is right, but it matches the swap chain setup
	if( GraphicsFamilyIndex != PresentFamilyIndex )
	{
		depthImageCreateInfo.sharingMode = VK_SHARING_MODE_CONCURRENT;
	}
	else
	{
		depthImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	VERIFY( VK_SUCCESS == vkCreateImage( LogicalDevice, &depthImageCreateInfo, NULL, &DepthBuffer ) );

	//Back the depth image with memory
	VkMemoryRequirements memoryRequirements;
	vkGetImageMemoryRequirements( LogicalDevice, DepthBuffer, &memoryRequirements );

	DepthBufferMemory = MemoryAllocator.Allocate( memoryRequirements.size, memoryRequirements.alignment, memoryRequirements.memoryTypeBits, 0, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT );

	VERIFY( VK_SUCCESS == vkBindImageMemory( LogicalDevice, DepthBuffer, DepthBufferMemory.DeviceMemory, DepthBufferMemory.Offset ) );

	VkImageViewCreateInfo depthViewCreateInfo = {};

	depthViewCreateInfo.sType =VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
	depthViewCreateInfo.image = DepthBuffer;
	depthViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
	depthViewCreateInfo.format = DepthFormat;
	depthViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
	depthViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
	depthViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
	depthViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;
	depthViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	depthViewCreateInfo.subresourceRange.baseMipLevel = 0;
	depthViewCreateInfo.subresourceRange.levelCount = 1;
	depthViewCreateInfo.subresourceRange.baseArrayLayer = 0;
	depthViewCreateInfo.subresourceRange.layerCount = 1;
	depthViewCreateInfo.flags = 0;

	VERIFY( VK_SUCCESS == vkCreateImageView( LogicalDevice, &depthViewCreateInfo, NULL, &DepthBufferView ) );
}

void GraphicsVK::CreateRenderPass( void )
{
	std::vector<VkAttachmentDescription> attachments;

	VkAttachmentDescription colourAttachment = {};

	colourAttachment.format = SwapChainFormat;
	colourAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
	colourAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
	colourAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	colourAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

	attachments.push_back( colourAttachment );

	VkAttachmentDescription depthAttachment = {};

	depthAttachment.format = DepthFormat;
	depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
	depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
	depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
	depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
	depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	attachments.push_back( depthAttachment );

	//Sub Pass
	VkAttachmentReference colourRef = {};

	colourRef.attachment = 0;
	colourRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

	VkAttachmentReference depthRef = {};

	depthRef.attachment = 1;
	depthRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

	VkSubpassDescription subpass = {};

	subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //Graphics or Compute?
	subpass.colorAttachmentCount = 1;
	subpass.pColorAttachments = &colourRef;
	subpass.pDepthStencilAttachment = &depthRef;

	VkRenderPassCreateInfo renderPassCreateInfo = {};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = attachments.size();
	renderPassCreateInfo.pAttachments = attachments.data();
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpass;
	renderPassCreateInfo.dependencyCount = 0;

	VkSubpassDependency dependency = {};
	dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	dependency.dstSubpass = 0;
	dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.srcAccessMask = 0;
	dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &dependency;

	VERIFY( VK_SUCCESS == vkCreateRenderPass( LogicalDevice, &renderPassCreateInfo, NULL, &RenderPass ) );
}

std::vector<char> ReadFile( const std::string& Filename )
{
	std::ifstream file( Filename, std::ios::ate | std::ios::binary );

	if( !file.is_open() )
	{
	    throw std::runtime_error("failed to open file!");
	}

	size_t fileSize = (size_t)file.tellg();

	std::vector<char> buffer( fileSize );

	file.seekg(0);

	file.read( buffer.data(), fileSize );

	file.close();

	return buffer;
}

void GraphicsVK::CreatePipeline( void )
{
	auto vertShader = ReadFile( "vert.spv" );
	auto fragShader = ReadFile( "frag.spv" );

	VkShaderModuleCreateInfo shaderCreateInfo = {};
	shaderCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

	shaderCreateInfo.codeSize = vertShader.size();
	shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(vertShader.data());

	VERIFY( VK_SUCCESS == vkCreateShaderModule( LogicalDevice, &shaderCreateInfo, nullptr, &vertShaderModule ) );

	shaderCreateInfo.codeSize = fragShader.size();
	shaderCreateInfo.pCode = reinterpret_cast<const uint32_t*>(fragShader.data());

	VERIFY( VK_SUCCESS == vkCreateShaderModule( LogicalDevice, &shaderCreateInfo, nullptr, &fragShaderModule ) );

	VkPipelineShaderStageCreateInfo vertShaderStageInfo = {};

	vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertShaderStageInfo.module = vertShaderModule;
	vertShaderStageInfo.pName = "main";

	VkPipelineShaderStageCreateInfo fragShaderStageInfo = {};

	fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragShaderStageInfo.module = fragShaderModule;
	fragShaderStageInfo.pName = "main";

	std::vector<VkPipelineShaderStageCreateInfo> shaderStages = { vertShaderStageInfo, fragShaderStageInfo };

	auto bindingDescription = Vertex::GetBindingDescription();
	auto attributeDescriptions = Vertex::GetAttributeDescriptions();

	VkPipelineVertexInputStateCreateInfo vertexInputInfo = {};

	vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputInfo.vertexBindingDescriptionCount = 1;
	vertexInputInfo.vertexAttributeDescriptionCount = static_cast<uint32_t>( attributeDescriptions.size() );
	vertexInputInfo.pVertexBindingDescriptions = &bindingDescription;
	vertexInputInfo.pVertexAttributeDescriptions = attributeDescriptions.data();

	VkPipelineInputAssemblyStateCreateInfo inputAssembly = {};
	inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssembly.primitiveRestartEnable = VK_FALSE;

	VkViewport viewport = {};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = (float) SwapChainExtent.width;
	viewport.height = (float) SwapChainExtent.height;
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor = {};
	scissor.offset = {0, 0};
	scissor.extent = SwapChainExtent;

	VkPipelineViewportStateCreateInfo viewportState = {};
	viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportState.viewportCount = 1;
	viewportState.pViewports = &viewport;
	viewportState.scissorCount = 1;
	viewportState.pScissors = &scissor;

	VkPipelineRasterizationStateCreateInfo rasterizer = {};
	rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizer.depthClampEnable = VK_FALSE;
	rasterizer.rasterizerDiscardEnable = VK_FALSE;
	rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizer.lineWidth = 1.0f;
	rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizer.depthBiasEnable = VK_FALSE;
	rasterizer.depthBiasConstantFactor = 0.0f; // Optional
	rasterizer.depthBiasClamp = 0.0f; // Optional
	rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

	VkPipelineMultisampleStateCreateInfo multisampling = {};
	multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	multisampling.sampleShadingEnable = VK_FALSE;
	multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	multisampling.minSampleShading = 1.0f; // Optional
	multisampling.pSampleMask = nullptr; // Optional
	multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
	multisampling.alphaToOneEnable = VK_FALSE; // Optional

	VkPipelineColorBlendAttachmentState colorBlendAttachment = {};
	colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachment.blendEnable = VK_FALSE;
	colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
	colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
	colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
	colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional

	VkPipelineColorBlendStateCreateInfo colorBlending = {};
	colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlending.logicOpEnable = VK_FALSE;
	colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
	colorBlending.attachmentCount = 1;
	colorBlending.pAttachments = &colorBlendAttachment;
	colorBlending.blendConstants[0] = 0.0f; // Optional
	colorBlending.blendConstants[1] = 0.0f; // Optional
	colorBlending.blendConstants[2] = 0.0f; // Optional
	colorBlending.blendConstants[3] = 0.0f; // Optional

	VkStencilOpState front = {}, back = {};

	front.failOp = VK_STENCIL_OP_KEEP;
    front.passOp = VK_STENCIL_OP_KEEP;
    front.depthFailOp = VK_STENCIL_OP_KEEP;
    front.compareOp = VK_COMPARE_OP_ALWAYS;
    front.compareMask = 0;
    front.writeMask = 0;
    front.reference = 0;

	VkPipelineDepthStencilStateCreateInfo depth = {};

	depth.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
	depth.depthTestEnable = VK_FALSE;
	depth.depthWriteEnable = VK_FALSE;
	depth.depthCompareOp = VK_COMPARE_OP_ALWAYS ;
	depth.depthBoundsTestEnable = VK_FALSE;
	depth.stencilTestEnable = VK_FALSE;
	depth.front = front;
	depth.back = front;
	depth.minDepthBounds = 0.0f;
	depth.maxDepthBounds = 1.0f;

	VkPipelineLayoutCreateInfo PipelineLayoutInfo = {};
	PipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	PipelineLayoutInfo.setLayoutCount = 0; // Optional
	PipelineLayoutInfo.pSetLayouts = nullptr; // Optional
	PipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
	PipelineLayoutInfo.pPushConstantRanges = 0; // Optional

	VERIFY( VK_SUCCESS == vkCreatePipelineLayout( LogicalDevice, &PipelineLayoutInfo, nullptr, &PipelineLayout ) );

	VkGraphicsPipelineCreateInfo pipelineInfo = {};
	pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	pipelineInfo.stageCount = shaderStages.size();
	pipelineInfo.pStages = shaderStages.data();

	pipelineInfo.pVertexInputState = &vertexInputInfo;
	pipelineInfo.pInputAssemblyState = &inputAssembly;
	pipelineInfo.pViewportState = &viewportState;
	pipelineInfo.pRasterizationState = &rasterizer;
	pipelineInfo.pMultisampleState = &multisampling;
	pipelineInfo.pDepthStencilState = &depth; // Optional
	pipelineInfo.pColorBlendState = &colorBlending;
	pipelineInfo.pDynamicState = nullptr; // Optional

	pipelineInfo.layout = PipelineLayout;

	pipelineInfo.renderPass = RenderPass;
	pipelineInfo.subpass = 0;

	pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
	pipelineInfo.basePipelineIndex = -1; // Optional

	VERIFY( VK_SUCCESS == vkCreateGraphicsPipelines( LogicalDevice, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &GraphicsPipeline ) );
}

void GraphicsVK::CreateFrameBuffers( void )
{
	std::vector<VkImageView> attachments{2};

	attachments[1] = DepthBufferView;

	VkFramebufferCreateInfo frameBufferCreateInfo = {};

	frameBufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
	frameBufferCreateInfo.renderPass = RenderPass;
	frameBufferCreateInfo.attachmentCount = attachments.size();
	frameBufferCreateInfo.pAttachments = attachments.data();
	frameBufferCreateInfo.width = SwapChainExtent.width;
	frameBufferCreateInfo.height = SwapChainExtent.height;
	frameBufferCreateInfo.layers = 1;

	for( int ii = 0; ii < SwapChainImageViews.size(); ii++ )
	{
		attachments[0] = SwapChainImageViews[ii];

		VERIFY( VK_SUCCESS == vkCreateFramebuffer( LogicalDevice, &frameBufferCreateInfo, NULL, &FrameBuffers[ii] ) );
	}
}

void GraphicsVK::RecordCommands( void )
{
	for( int ii = 0; ii < CommandBuffers.size(); ii++ )
	{
		VkCommandBufferBeginInfo beginInfo = {};

		beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		beginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		beginInfo.pInheritanceInfo = nullptr; // Optional

		vkBeginCommandBuffer( CommandBuffers[ii], &beginInfo );

		VkRenderPassBeginInfo renderPassInfo = {};

		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassInfo.renderPass = RenderPass;
		renderPassInfo.framebuffer = FrameBuffers[ii];

		renderPassInfo.renderArea.offset = {0, 0};
		renderPassInfo.renderArea.extent = SwapChainExtent;

		VkClearValue clearColor = {0.0f, 0.0f, 0.0f, 1.0f};
		renderPassInfo.clearValueCount = 1;
		renderPassInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass( CommandBuffers[ii], &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE );

		vkCmdBindPipeline( CommandBuffers[ii], VK_PIPELINE_BIND_POINT_GRAPHICS, GraphicsPipeline );

		VkBuffer vertexBuffers[] = { VertexBuffer };
		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers( CommandBuffers[ii], 0, 1, vertexBuffers, offsets );

		vkCmdBindIndexBuffer( CommandBuffers[ii], IndexBuffer, 0, VK_INDEX_TYPE_UINT16 );

		vkCmdDrawIndexed( CommandBuffers[ii], static_cast<uint16_t>(drawIndices.size()), 1, 0, 0, 0 );

		//vkCmdDraw( CommandBuffers[ii], static_cast<uint32_t>( vertices.size() ), 1, 0, 0 );

		//vkCmdDraw( CommandBuffers[ii], 3, 1, 0, 0 );

		vkCmdEndRenderPass( CommandBuffers[ii] );

		VERIFY( VK_SUCCESS == vkEndCommandBuffer( CommandBuffers[ii] ) );
	}
}

void GraphicsVK::DrawFrame( void )
{
	uint32_t imageIndex;

	StagingManager.SubmitQueue();
	/*
	TODO
	Some of the functions below return VkResult, need to test the return values

	if (result == VK_ERROR_OUT_OF_DATE_KHR) {
	    recreateSwapChain();
	    return;
	} else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
	    throw std::runtime_error("failed to acquire swap chain image!");
	}

	if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) {
	    recreateSwapChain();
	} else if (result != VK_SUCCESS) {
	    throw std::runtime_error("failed to present swap chain image!");
	}
	*/

	//vkQueueWaitIdle( PresentQueue );
	VERIFY( VK_SUCCESS == vkQueueWaitIdle( PresentQueue ) );

	vkAcquireNextImageKHR( LogicalDevice, SwapChain, std::numeric_limits<uint64_t>::max(), AcquireSemaphores[CurrentFrame], VK_NULL_HANDLE, &imageIndex );

	VkSubmitInfo submitInfo = {};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;

	VkSemaphore waitSemaphores[] = {AcquireSemaphores[CurrentFrame]};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffers[imageIndex];

	VkSemaphore signalSemaphores[] = {RenderCompleteSemaphores[CurrentFrame]};
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	VERIFY( VK_SUCCESS == vkQueueSubmit( GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE ) );

	VkPresentInfoKHR presentInfo = {};

	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { SwapChain };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR( PresentQueue, &presentInfo );

	CurrentFrame = ( CurrentFrame + 1 ) % BufferCount;
}

VkBuffer GraphicsVK::CreateBuffer( VkDeviceSize Size, VkBufferUsageFlags Usage, VkSharingMode SharingMode, VkMemoryPropertyFlags Properties, vkAllocation& Allocation )
{
	VkBuffer buffer;

    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = Size;
    bufferInfo.usage = Usage;
    bufferInfo.sharingMode = SharingMode; //VK_SHARING_MODE_EXCLUSIVE;

    VERIFY( VK_SUCCESS == vkCreateBuffer( LogicalDevice, &bufferInfo, nullptr, &buffer ) );

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements( LogicalDevice, buffer, &memRequirements );

    Allocation = MemoryAllocator.Allocate( memRequirements.size, memRequirements.alignment, memRequirements.memoryTypeBits, Properties, 0 );

    VERIFY( VK_SUCCESS == vkBindBufferMemory( LogicalDevice, buffer, Allocation.DeviceMemory, Allocation.Offset ) );

    return buffer;
}

void GraphicsVK::CreateVertexBuffer( void )
{
	VkDeviceSize bufferSize = sizeof(vertices[0]) * vertices.size();

	vkAllocation stagingBufferMemory;
	
	VkBuffer stagingBuffer = StagingManager.Stage( bufferSize, stagingBufferMemory );

	memcpy( stagingBufferMemory.Data, vertices.data(), (size_t) bufferSize );

	VertexBuffer = CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		VertexBufferMemory
		);

	StagingManager.CopyBuffer( stagingBuffer, VertexBuffer, bufferSize );

	//MemoryAllocator.Free( stagingBufferMemory );

	//vkDestroyBuffer( LogicalDevice, stagingBuffer, nullptr );
}

void GraphicsVK::CreateIndexBuffer( void )
{
	VkDeviceSize bufferSize = sizeof(drawIndices[0]) * drawIndices.size();

    vkAllocation stagingBufferMemory;
	
	VkBuffer stagingBuffer = StagingManager.Stage( bufferSize, stagingBufferMemory );

    memcpy( stagingBufferMemory.Data, drawIndices.data(), (size_t) bufferSize );

    IndexBuffer = CreateBuffer(
		bufferSize,
		VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		VK_SHARING_MODE_EXCLUSIVE,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
		IndexBufferMemory
		);

    StagingManager.CopyBuffer( stagingBuffer, IndexBuffer, bufferSize );
}
/*
void GraphicsVK::CopyBuffer( VkBuffer Source, VkBuffer Destination, VkDeviceSize Size )
{
	VkCommandBufferAllocateInfo allocInfo = {};

	allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	allocInfo.commandPool = CommandPool;
	allocInfo.commandBufferCount = 1;

	VkCommandBuffer CommandBuffer;

	vkAllocateCommandBuffers( LogicalDevice, &allocInfo, &CommandBuffer );

	VkCommandBufferBeginInfo beginInfo = {};

	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

	VERIFY( VK_SUCCESS == vkBeginCommandBuffer( CommandBuffer, &beginInfo ) );

	VkBufferCopy copyRegion = {};

	copyRegion.srcOffset = 0; // Optional
	copyRegion.dstOffset = 0; // Optional
	copyRegion.size = Size;
	
	vkCmdCopyBuffer( CommandBuffer, Source, Destination, 1, &copyRegion );

	VERIFY( VK_SUCCESS == vkEndCommandBuffer( CommandBuffer ) );

	VkSubmitInfo submitInfo = {};

	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &CommandBuffer;

	VERIFY( VK_SUCCESS == vkQueueSubmit( GraphicsQueue, 1, &submitInfo, VK_NULL_HANDLE ) );

	VERIFY( VK_SUCCESS == vkQueueWaitIdle( GraphicsQueue ) );

	vkFreeCommandBuffers( LogicalDevice, CommandPool, 1, &CommandBuffer );
}
//*/
VkResult GraphicsVK::FindMemoryTypeIndex( const unsigned int MemoryTypeBits, const VkMemoryPropertyFlags Required, const VkMemoryPropertyFlags Preferred, unsigned int& SelectedMemoryTypeIndex, VkMemoryPropertyFlags& SupportedProperties )
{
	ASSERT( MemoryTypeBits != 0 );

	const VkPhysicalDeviceMemoryProperties& physicalMemoryProperties = GPUInfo->MemoryProperties;

	const VkMemoryPropertyFlags required = Required;

	const VkMemoryPropertyFlags preferred = Preferred | Required;

	//preferred must be a superset of required.
    ASSERT( ( required & ~preferred ) == 0 );

    SelectedMemoryTypeIndex = UINT32_MAX;
    unsigned int minCost = UINT32_MAX;

    //Search for the memory type that satisfies ALL the required flags, and the MOST preferred flags
    for( unsigned int memTypeIndex = 0, memTypeBit = 1; memTypeIndex < physicalMemoryProperties.memoryTypeCount; memTypeIndex++, memTypeBit <<= 1 )
    {
    	if( ( memTypeBit & MemoryTypeBits ) != 0 )
    	{
    		const VkMemoryPropertyFlags currentFlags = physicalMemoryProperties.memoryTypes[memTypeIndex].propertyFlags;

    		//If this memory type has all the required flags
    		if( ( required & ~currentFlags ) == 0 )
    		{
    			//Determine how many preferred flags it doesn't have
    			std::bitset<sizeof(VkMemoryPropertyFlags)> missingFlags(preferred & ~currentFlags);
    			unsigned int currentCost = missingFlags.count();
    			
    			//Store the properties of the current memory type
    			VkMemoryPropertyFlags availFlags = currentFlags; //preferred & currentFlags;

    			//Min search for memory type with fewest missing preferred flags
    			if( currentCost < minCost )
    			{
    				//Set the index AND record the supported properties
    				SelectedMemoryTypeIndex = memTypeIndex;
    				SupportedProperties = availFlags;

    				if( currentCost == 0 )
    				{
    					//Can't get better than this
    					return VK_SUCCESS;
    				}

    				minCost = currentCost;
    			}
    		}
    	}
    }

    //Didn't find an exact match, SelectedMemoryTypeIndex is now the closest

    //Check that any match was actually found
	return ( (SelectedMemoryTypeIndex != UINT32_MAX) ? VK_SUCCESS : VK_ERROR_FEATURE_NOT_PRESENT );
}

void GraphicsVK::ReadSettings( void )
{
	std::ifstream mainSettingsFile{ settingsFile };
	
	mainSettingsFile >> Settings;
}

void GraphicsVK::WriteSettings( void )
{
	std::ofstream mainSettingsFile{ settingsFile };

	mainSettingsFile << std::setw(4) << Settings << std::endl;
}