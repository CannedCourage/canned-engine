#include "Graphics/GraphicsVK.h"
#include "System/System.h"

#include <unordered_set>

static_assert( sizeof(unsigned int) == 4, "Vulkan relies on 32-bit integer data type" );

GraphicsVK::GraphicsVK( System &s ) : log("GraphicsVK"), system( s ), window( system.window )
{
}

GraphicsVK::~GraphicsVK( void )
{
}

void GraphicsVK::Init( void )
{
	TRACE( "GraphicsVK Init" );

	CreateInstance();

	CreateSurface();

	EnumeratePhysicalDevices();

	SelectPhysicalDevice();

	CreateLogicalDeviceAndQueues();

	CreateSemaphores();

	CreateCommandPool();

	CreateCommandBuffer();

	//vulkanAllocator.Init();

	//stagingManager.Init();

	CreateSwapChain();

	CreateRenderTargets();

	CreateRenderPass();

	//CreatePipeline();

	CreateFrameBuffers();
}

void GraphicsVK::CleanUp( void )
{
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

	InstanceExtensions.push_back( VK_KHR_SURFACE_EXTENSION_NAME );
	InstanceExtensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );

	DeviceExtensions.push_back( VK_KHR_WIN32_SURFACE_EXTENSION_NAME );

	if( enableDebugLayers )
	{
		InstanceExtensions.push_back( VK_EXT_DEBUG_REPORT_EXTENSION_NAME );

		ValidationLayers.push_back( "VK_LAYER_LUNARG_standard_validation" );
	}

	//ValidateValidationLayers();???

	createInfo.enabledExtensionCount = InstanceExtensions.size();
	createInfo.ppEnabledExtensionNames = InstanceExtensions.data();
	createInfo.enabledLayerCount = ValidationLayers.size();
	createInfo.ppEnabledLayerNames = ValidationLayers.data();

	VERIFY( VK_SUCCESS == vkCreateInstance( &createInfo, NULL, &Instance) );

	if( enableDebugLayers )
	{
		//CreateDebugReportCallback();???
	}
}

void GraphicsVK::CreateSurface( void )
{
	VkWin32SurfaceCreateInfoKHR createInfo = {};

	createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	//createInfo.hinstance = window.GetInstance();
	//createInfo.hwnd = window.GetHandle();

	VERIFY( VK_SUCCESS == vkCreateWin32SurfaceKHR( Instance, &createInfo, NULL, &Surface) );
	ASSERT( Surface != nullptr );
}

void GraphicsVK::EnumeratePhysicalDevices( void )
{
	unsigned int numDevices = 0;
	std::vector<VkPhysicalDevice> devices;

	VERIFY( VK_SUCCESS == vkEnumeratePhysicalDevices( Instance, &numDevices, NULL ) );
	ASSERT( numDevices > 0 );

	devices.reserve( numDevices );

	VERIFY( VK_SUCCESS == vkEnumeratePhysicalDevices( Instance, &numDevices, devices.data() ) );
	ASSERT( numDevices > 0 );

	GPUs.reserve( numDevices );

	for( int ii = 0; ii < numDevices; ii++ )
	{
		GPU& gpu = GPUs[ii];
		gpu.Device = devices[ii];

		{
			unsigned int numQueues = 0;

			vkGetPhysicalDeviceQueueFamilyProperties( gpu.Device, &numQueues, NULL );
			ASSERT( numQueues > 0 );

			gpu.QueueFamilyProperties.reserve( numQueues );

			vkGetPhysicalDeviceQueueFamilyProperties( gpu.Device, &numQueues, gpu.QueueFamilyProperties.data() );
			ASSERT( numQueues > 0 );
		}

		{
			unsigned int numExtensions = 0;

			VERIFY( VK_SUCCESS == vkEnumerateDeviceExtensionProperties( gpu.Device, NULL, &numExtensions, NULL ) );
			ASSERT( numExtensions > 0 );

			gpu.ExtensionProperties.reserve( numExtensions );

			VERIFY( VK_SUCCESS == vkEnumerateDeviceExtensionProperties( gpu.Device, NULL, &numExtensions, gpu.ExtensionProperties.data() ) );
			ASSERT( numExtensions > 0 );
		}

		VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceCapabilitiesKHR( gpu.Device, Surface, &(gpu.SurfaceCapabilities) ) );

		{
			unsigned int numFormats = 0;

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR( gpu.Device, Surface, &numFormats, NULL ) );
			ASSERT( numFormats > 0 );

			gpu.SurfaceFormats.reserve( numFormats );

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfaceFormatsKHR( gpu.Device, Surface, &numFormats, gpu.SurfaceFormats.data() ) );
			ASSERT( numFormats > 0 );
		}

		{
			unsigned int numPresentModes = 0;

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR( gpu.Device, Surface, &numPresentModes, NULL ) );
			ASSERT( numPresentModes > 0 );

			gpu.PresentModes.reserve( numPresentModes );

			VERIFY( VK_SUCCESS == vkGetPhysicalDeviceSurfacePresentModesKHR( gpu.Device, Surface, &numPresentModes, gpu.PresentModes.data() ) );
			ASSERT( numPresentModes > 0 );
		}

		vkGetPhysicalDeviceMemoryProperties( gpu.Device, &(gpu.MemoryProperties) );

		vkGetPhysicalDeviceProperties( gpu.Device, &(gpu.DeviceProperties) );
	}
}

void GraphicsVK::SelectPhysicalDevice( void )
{
	for( GPU& gpu : GPUs )
	{
		int graphicsIdx = -1;
		int presentIdx = -1;

		//???
		/*
		if ( !CheckPhysicalDeviceExtensionSupport( gpu, DeviceExtensions ) ) {
			continue;
		}
		*/

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

		if( graphicsIdx >= 0 && presentIdx >= 0 )
		{
			GraphicsFamilyIndex = graphicsIdx;
			PresentFamilyIndex = presentIdx;
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
	
	VERIFY( VK_SUCCESS == vkCreateDevice( PhysicalDevice, &info, NULL, &LogicalDevice ) );

	vkGetDeviceQueue( LogicalDevice, GraphicsFamilyIndex, 0, &GraphicsQueue );
	vkGetDeviceQueue( LogicalDevice, PresentFamilyIndex, 0, &PresentQueue );
}

void GraphicsVK::CreateSemaphores( void )
{
	VkSemaphoreCreateInfo semaphoreCreateInfo = {};

	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

	for( int ii = 0; ii < bufferCount; ii++ )
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
	commandBufferAllocateInfo.commandBufferCount = bufferCount;

	VERIFY( VK_SUCCESS == vkAllocateCommandBuffers( LogicalDevice, &commandBufferAllocateInfo, CommandBuffer.data() ) );

	VkFenceCreateInfo fenceCreateInfo = {};
	fenceCreateInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

	for( int ii = 0; ii < bufferCount; ii++ )
	{
		VERIFY( VK_SUCCESS == vkCreateFence( LogicalDevice, &fenceCreateInfo, NULL, &CommandBufferFences[ii] ) );
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
		extent.width = 800;
		extent.height = 600;
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

	swapChainCreateInfo.minImageCount = bufferCount;

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
	std::vector<VkImage> swapChainImages{ bufferCount };

	VERIFY( VK_SUCCESS == vkGetSwapchainImagesKHR( LogicalDevice, SwapChain, &numImages, NULL ) );
	ASSERT( numImages > 0 );

	swapChainImages.reserve( numImages );

	VERIFY( VK_SUCCESS == vkGetSwapchainImagesKHR( LogicalDevice, SwapChain, &numImages, swapChainImages.data() ) );
	ASSERT( numImages > 0 );

	for( int ii = 0; ii < bufferCount; ii++ )
	{
		VkImageViewCreateInfo imageViewCreateInfo = {};

		imageViewCreateInfo.sType =VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.image = swapChainImages[ii];
		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = SwapChainFormat;
		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
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
	depthImageCreateInfo.initialLayout = VK_IMAGE_LAYOUT_PREINITIALIZED;
	depthImageCreateInfo.usage = VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
	
	//???
	depthImageCreateInfo.samples = VK_SAMPLE_COUNT_1_BIT;
	depthImageCreateInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

	VERIFY( VK_SUCCESS == vkCreateImage( LogicalDevice, &depthImageCreateInfo, NULL, &DepthBuffer ) );

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
	colourAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
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

	VERIFY( VK_SUCCESS == vkCreateRenderPass( LogicalDevice, &renderPassCreateInfo, NULL, &RenderPass ) );
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

	for( int ii = 0; ii < bufferCount; ii++ )
	{
		attachments[0] = SwapChainImageViews[ii];

		VERIFY( VK_SUCCESS == vkCreateFramebuffer( LogicalDevice, &frameBufferCreateInfo, NULL, &FrameBuffers[ii] ) );
	}
}

void GraphicsVK::ReadSettings( void )
{
	/*
	fullscreen = system.GlobalSettings["display"]["fullscreen"];
	xResolution = system.GlobalSettings["display"]["xResolution"];
	yResolution = system.GlobalSettings["display"]["yResolution"];
	aspect = (ASPECT)system.GlobalSettings["display"]["aspect"];
	refresh = system.GlobalSettings["display"]["refresh"];
	AA = (D3DMULTISAMPLE_TYPE)system.GlobalSettings["display"]["multisample"];
	mode = system.GlobalSettings["display"]["displayMode"];
	backbufferFormat = (D3DFORMAT)system.GlobalSettings["display"]["bufferFormat"];
	depthFormat = (D3DFORMAT)system.GlobalSettings["display"]["depthFormat"];
	adapter = system.GlobalSettings["display"]["adapter"];
	*/
}

void GraphicsVK::WriteSettings( void )
{
	/*
	system.GlobalSettings["display"]["fullscreen"] = fullscreen;
	system.GlobalSettings["display"]["xResolution"] = xResolution;
	system.GlobalSettings["display"]["yResolution"] = yResolution;
	system.GlobalSettings["display"]["aspect"] = aspect;
	system.GlobalSettings["display"]["refresh"] = refresh;
	system.GlobalSettings["display"]["multisample"] = AA;
	system.GlobalSettings["display"]["displayMode"] = mode;
	system.GlobalSettings["display"]["bufferFormat"] = backbufferFormat;
	system.GlobalSettings["display"]["depthFormat"] = depthFormat;
	system.GlobalSettings["display"]["bufferCount"] = bufferCount;
	system.GlobalSettings["display"]["adapter"] = adapter;
	*/
}