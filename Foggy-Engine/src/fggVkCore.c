#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "fggVkCore.h"
#include "fggWindow.h"
#include "fggUtilities.h"

#include "fggComponents.h"

#pragma warning (disable: 6011)
#pragma warning (disable: 6385)
#pragma warning (disable: 6386)
#pragma warning (disable: 6255)

/*
* First setup
*/

FggVkCore fggVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char *title) {
	
	FggVkCore core = {	
		0,							//instance;
		0,							//physical_device;
		0,							//device;
		{NULL,
		width, 
		height, 
		title},						//window;
		0,							//surface;
		VK_QUEUE_GRAPHICS_BIT,		//requiredQueueFlag;
		1,							//queue_family_index_count
		0,							//graphics_queue_index;
		0,							//present_queue_index;
		0,							//graphicsQueue;
		NULL,						//p_cmd_pools;
		0,							//p_cmd_buffer;
		0,							//swapchain;
		VK_FORMAT_R8G8B8A8_UNORM,	//imageFormat;
		0,							//swapchain_image_count;
		NULL,						//p_swapchain_images;
		NULL,						//p_swapchain_image_views;
		NULL,						//p_frame_buffers;
		0,							//renderPass;
		0,							//render_semaphore;
		0,							//presentSemaphore;
		0,							//render_fence;
	};

	fggInitGLFW(&core.window);
	
	return core;
}

void fggInitVulkan(FggVkCore *core) {
	fggCreateInstance(core);
	fggCreateWindowSurface(core->instance, core->window.window, &core->surface);
	fggSetPhysicalDevice(core);
	fggSetLogicalDevice(core);
	fggGetGraphicsQueue(core);
}

void fggCreateInstance(FggVkCore* core) {

	VkApplicationInfo application_info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,	//sType;
		NULL,								//pNext;
		"VK-Compute Application",			//pApplicationName;
		VK_MAKE_VERSION(0, 1, 0),			//applicationVersion;
		"VK-Compute Engine",				//pEngineName;
		VK_MAKE_VERSION(0, 1, 0),			//engineVersion;
		FGG_VULKAN_VERSION,					//apiVersion;
	};

	const char* khronos_validation = "VK_LAYER_KHRONOS_validation";

	uint32_t instance_extensions_count = 0;
	const char** glfw_extension_names = glfwGetRequiredInstanceExtensions(&instance_extensions_count);

	VkInstanceCreateInfo instance_create_info = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		&application_info,						//pApplicationInfo;
		0,										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		instance_extensions_count,				//enabledExtensionCount;
		glfw_extension_names					//ppEnabledExtensionNames;
	};

#ifndef NDEBUG
		if (fggCheckValidationLayer(khronos_validation)) {
			instance_create_info.enabledLayerCount = 1;
			instance_create_info.ppEnabledLayerNames = &khronos_validation;
		}

		puts("creating vkinstance");
#endif

	fggCheckVkResult(
		vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, &core->instance),
		"error creating vkinstance"
	);
}

void fggCreateWindowSurface(const VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface) {
	
	fggCheckVkResult(
		glfwCreateWindowSurface(instance, window, NULL, surface),
		"error creating window surface"
	);
}

extern VkSurfaceCapabilitiesKHR fggGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &surfaceCapabilities);
	return surfaceCapabilities;
}

void fggSetPhysicalDevice(FggVkCore* core) {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(core->instance, &pDeviceCount, NULL);

	VkPhysicalDevice *pDevices = (VkPhysicalDevice*)malloc(pDeviceCount*sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(core->instance, &pDeviceCount, pDevices);

	if (pDeviceCount == 0) {
		printf("No Vulkan compatible gpu has been found");
		exit(-1);
	}

	uint32_t* graphicsQueueFamilyIndices = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t* surfaceQueueFamilyIndices  = (uint32_t*)malloc(pDeviceCount * sizeof(uint32_t));
	uint32_t suitableDeviceCount = 0;

	for (uint32_t i = 0; i < pDeviceCount; i++) { //DEVICE LOOP

		uint32_t queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, NULL);
		VkQueueFamilyProperties* pQueueFamilyProperties = (VkQueueFamilyProperties*)malloc(queueFamilyPropertyCount * sizeof(VkQueueFamilyProperties));
		vkGetPhysicalDeviceQueueFamilyProperties(pDevices[i], &queueFamilyPropertyCount, pQueueFamilyProperties);
		
		VkBool32 surfaceSupport = 0;

		for (uint32_t j = 0; j < queueFamilyPropertyCount; j++) {

			//SURFACE SUPPORT
			if (!surfaceSupport) {
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, core->surface, &surfaceSupport);
				if (surfaceSupport) {
					surfaceQueueFamilyIndices[i] = j;
				}
			}

			// GRAPHICS QUEUE
			if (pQueueFamilyProperties[j].queueFlags & core->required_queue_flag) {
				graphicsQueueFamilyIndices[i] = j;
			}

			//SUITABLE
			if (pQueueFamilyProperties[j].queueFlags & core->required_queue_flag && surfaceSupport) {
				suitableDeviceCount += 1;
				break;
			}
		}

		free(pQueueFamilyProperties);
	}

	if (suitableDeviceCount == 0) {
		puts("No suitable device has been found");
		exit(-1);
	}

	uint32_t *scores = malloc(suitableDeviceCount * sizeof(uint32_t));
	for (uint32_t i = 0; i < suitableDeviceCount; i++) {

		scores[i] = 0;

		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevices[i], &pDeviceProperties);

		VkPhysicalDeviceFeatures pDeviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevices[i], &pDeviceFeatures);
		
		scores[i] += pDeviceProperties.limits.maxComputeSharedMemorySize;
		scores[i] += pDeviceProperties.limits.maxComputeWorkGroupInvocations;

	}

	if (suitableDeviceCount > 1) {

		for (uint32_t i = 1; i < suitableDeviceCount; i++) {
			if (scores[i] > scores[i - 1]) {
				core->physical_device = pDevices[i];
				core->graphics_queue_index = graphicsQueueFamilyIndices[i];
				core->present_queue_index = surfaceQueueFamilyIndices[i];
			}
			else {
				core->physical_device = pDevices[i - 1 ];
				core->graphics_queue_index = graphicsQueueFamilyIndices[i - 1];
				core->present_queue_index = surfaceQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		core->physical_device = pDevices[0];
		core->graphics_queue_index = graphicsQueueFamilyIndices[0];
		core->present_queue_index = surfaceQueueFamilyIndices[0];
	}
	
	if (core->graphics_queue_index == core->present_queue_index) {
		core->queue_family_index_count = 1;
	}
	else {
		core->queue_family_index_count = 2;
	}
	free(scores);
	free(graphicsQueueFamilyIndices);
	free(surfaceQueueFamilyIndices);
	free(pDevices);

#ifndef NDEBUG
		VkPhysicalDeviceProperties physical_device_properties;
		vkGetPhysicalDeviceProperties(core->physical_device, &physical_device_properties);
		printf("using %s \n", physical_device_properties.deviceName);
#endif

}

/*
*	Logical device creation + command buffers
*/

VkDeviceQueueCreateInfo fggSetQueueInfo(const uint32_t queueFamilyIndex, const float *priority) {
	
	VkDeviceQueueCreateInfo queueCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		queueFamilyIndex,							//queueFamilyIndex;
		1,											//queueCount;
		priority,									//pQueuePriorities;
	};
	
	return queueCreateInfo;
}

void fggSetLogicalDevice(FggVkCore *core) {
	
	const float queue_priority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueInfo = fggSetQueueInfo(core->graphics_queue_index, &queue_priority);

	const char* swapchain_extension_name = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		1, 										//queueCreateInfoCount;
		&graphicsQueueInfo,						//pQueueCreateInfos;
		0, 										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		1, 										//enabledExtensionCount;
		&swapchain_extension_name,				//ppEnabledExtensionNames;
		NULL									//pEnabledFeatures;
	};

#ifndef NDEBUG
	puts("creating logical device");
#endif

	fggCheckVkResult(
		vkCreateDevice(core->physical_device, &deviceCreateInfo, NULL, &core->device),
		"error creating logical device"
	);
		
}

void fggGetGraphicsQueue(FggVkCore *core) {
	vkGetDeviceQueue(core->device, core->graphics_queue_index, 0, &core->graphics_queue);
}

void fggInitSwapchainData(FggVkCore *core) {
	fggCreateSwapchain(core);
	fggGetSwapchainImages(core);
	fggCreateSwapchainImageViews(core);
}

void fggCreateSwapchain(FggVkCore* core) {
/*
	VkSurfaceCapabilitiesKHR sCapabilities = fggGetSurfaceCapabilities(core->physical_device, core->surface);

	uint32_t surfaceFormatCount = 0;
	vkGetphysical_deviceSurfaceFormatsKHR(core->physical_device, core->surface, &surfaceFormatCount, NULL);
	VkSurfaceFormatKHR* pSurfaceFormats = (VkSurfaceFormatKHR*)malloc(surfaceFormatCount * sizeof(VkSurfaceFormatKHR));
	vkGetphysical_deviceSurfaceFormatsKHR(core->physical_device, core->surface, &surfaceFormatCount, pSurfaceFormats);

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,	//sType;
		NULL,											//pNext;
		0,												//flags;
		core->surface,									//surface;
		sCapabilities.minImageCount,					//minImageCount;
		core->imageFormat,								//imageFormat;
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,				//imageColorSpace;
		sCapabilities.currentExtent,					//imageExtent;
		1,												//imageArrayLayers;
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			//imageUsage;
		VK_SHARING_MODE_EXCLUSIVE,						//imageSharingMode;
		1,												//queue_family_index_count;
		&core->graphics_queue_index,						//pQueueFamilyIndices;
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,			//preTransform;
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,				//compositeAlpha;
		VK_PRESENT_MODE_FIFO_KHR,						//presentMode;
		1,												//clipped;
		0,												//oldSwapchain;
	};

	//FORMAT CHECK
	{
		int found = 0;
		for (uint32_t i = 0; i < surfaceFormatCount; i++) {
			if (pSurfaceFormats[i].format == core->imageFormat) {
				found = 1; break;
			}
		}
		if (found == 0) {
			core->imageFormat = pSurfaceFormats[0].format;
			swapchainCreateInfo.imageFormat = core->imageFormat;
		}
	}

	//COLORSPACE CHECK
	{
		int found = 0;
		for (uint32_t i = 0; i < surfaceFormatCount; i++) {
			if (pSurfaceFormats[i].format == core->imageFormat) {
				found = 1; break;
			}
		}
		if (found == 0) {
			core->imageFormat = pSurfaceFormats[0].format;
			swapchainCreateInfo.imageFormat = core->imageFormat;
		}
	}
	free(pSurfaceFormats);
	
	//SHARING MODE CHECK
	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(2 * sizeof(uint32_t));
	{
		if (core->graphics_queue_index != core->present_queue_index) {
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queue_family_index_count = 2;
			pQueueFamilyIndices[0] = core->graphics_queue_index; 
			pQueueFamilyIndices[1] = core->present_queue_index;
			swapchainCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
		}
	}
	free(pQueueFamilyIndices);

#ifndef NDEBUG
	puts("creating swapchain");
#endif

	fggCheckVkResult(
		vkCreateSwapchainKHR(core->device, &swapchainCreateInfo, NULL, &core->swapchain),
		"error creating swapchain"
	);
*/
	// Get the list of VkFormats that are supported:
    uint32_t formatCount;
    int res = vkGetPhysicalDeviceSurfaceFormatsKHR(core->physical_device, core->surface, &formatCount, NULL);
    assert(res == VK_SUCCESS);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(core->physical_device, core->surface, &formatCount, surfFormats);
    assert(res == VK_SUCCESS);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        core->image_format = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
        assert(formatCount >= 1);
        core->image_format = surfFormats[0].format;
    }
    free(surfFormats);

    VkSurfaceCapabilitiesKHR surfCapabilities;

    res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(core->physical_device, core->surface, &surfCapabilities);
    assert(res == VK_SUCCESS);

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(core->physical_device, core->surface, &presentModeCount, NULL);
    assert(res == VK_SUCCESS);
    VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));

    res = vkGetPhysicalDeviceSurfacePresentModesKHR(core->physical_device, core->surface, &presentModeCount, presentModes);
    assert(res == VK_SUCCESS);

    VkExtent2D swapchainExtent;
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = core->window.width;
        swapchainExtent.height = core->window.height;
        if (swapchainExtent.width < surfCapabilities.minImageExtent.width) {
            swapchainExtent.width = surfCapabilities.minImageExtent.width;
        } else if (swapchainExtent.width > surfCapabilities.maxImageExtent.width) {
            swapchainExtent.width = surfCapabilities.maxImageExtent.width;
        }

        if (swapchainExtent.height < surfCapabilities.minImageExtent.height) {
            swapchainExtent.height = surfCapabilities.minImageExtent.height;
        } else if (swapchainExtent.height > surfCapabilities.maxImageExtent.height) {
            swapchainExtent.height = surfCapabilities.maxImageExtent.height;
        }
    } else {
        // If the surface size is defined, the swap chain size must match
        swapchainExtent = surfCapabilities.currentExtent;
    }

    // The FIFO present mode is guaranteed by the spec to be supported
    VkPresentModeKHR swapchainPresentMode = VK_PRESENT_MODE_FIFO_KHR;

    // Determine the number of VkImage's to use in the swap chain.
    // We need to acquire only 1 presentable image at at time.
    // Asking for minImageCount images ensures that we can acquire
    // 1 presentable image as long as we present it before attempting
    // to acquire another.
    uint32_t desiredNumberOfSwapChainImages = surfCapabilities.minImageCount;

    VkSurfaceTransformFlagBitsKHR preTransform;
    if (surfCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
        preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
    } else {
        preTransform = surfCapabilities.currentTransform;
    }

    // Find a supported composite alpha mode - one of these is guaranteed to be set
    VkCompositeAlphaFlagBitsKHR compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    VkCompositeAlphaFlagBitsKHR compositeAlphaFlags[4] = {
        VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR,
        VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
    };
    for (uint32_t i = 0; i < sizeof(compositeAlphaFlags) / sizeof(compositeAlphaFlags[0]); i++) {
        if (surfCapabilities.supportedCompositeAlpha & compositeAlphaFlags[i]) {
            compositeAlpha = compositeAlphaFlags[i];
            break;
        }
    }

    VkSwapchainCreateInfoKHR swapchain_ci = { 0 };
    swapchain_ci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    swapchain_ci.pNext = NULL;
    swapchain_ci.surface = core->surface;
    swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
    swapchain_ci.imageFormat = core->image_format;
    swapchain_ci.imageExtent.width = swapchainExtent.width;
    swapchain_ci.imageExtent.height = swapchainExtent.height;
    swapchain_ci.preTransform = preTransform;
    swapchain_ci.compositeAlpha = compositeAlpha;
    swapchain_ci.imageArrayLayers = 1;
    swapchain_ci.presentMode = swapchainPresentMode;
    swapchain_ci.oldSwapchain = VK_NULL_HANDLE;
    swapchain_ci.clipped = true;
    swapchain_ci.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
    swapchain_ci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
    swapchain_ci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    swapchain_ci.queueFamilyIndexCount = 0;
    swapchain_ci.pQueueFamilyIndices = NULL;
    uint32_t queueFamilyIndices[2] = {(uint32_t)core->graphics_queue_index, (uint32_t)core->present_queue_index};
    if (core->graphics_queue_index != core->present_queue_index) {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }

	fggCheckVkResult(
		vkCreateSwapchainKHR(core->device, &swapchain_ci, NULL, &core->swapchain),
		"error creating swapchain"
	);
}

void fggGetSwapchainImages(FggVkCore *core) {
	vkGetSwapchainImagesKHR(core->device, core->swapchain, &core->swapchain_image_count, NULL);
	core->p_swapchain_images = (VkImage*)malloc(core->swapchain_image_count * sizeof(VkImage));
	vkGetSwapchainImagesKHR(core->device, core->swapchain, &core->swapchain_image_count, core->p_swapchain_images);
}

void fggCreateSwapchainImageViews(FggVkCore *core) {
	core->p_swapchain_image_views = (VkImageView*)malloc(core->swapchain_image_count * sizeof(VkImageView));

	for (uint32_t i = 0; i < core->swapchain_image_count; i++) {
		
		VkImageViewCreateInfo imageViewCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	//sType;
			NULL,										//pNext;
			0,											//flags;
			core->p_swapchain_images[i],					//image;
			VK_IMAGE_VIEW_TYPE_2D,						//viewType;
			core->image_format,							//format;
			VK_COMPONENT_SWIZZLE_IDENTITY,				//components;
			0
		};

		imageViewCreateInfo.subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;	//aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;							//baseMipLevel;
		imageViewCreateInfo.subresourceRange.levelCount = 1;							//levelCount;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;						//baseArrayLayer;
		imageViewCreateInfo.subresourceRange.layerCount = 1;							//layerCount;

		fggCheckVkResult(
			vkCreateImageView(core->device, &imageViewCreateInfo, NULL, &core->p_swapchain_image_views[i]),
			"error creating image view"
		);
	}
}

void fggInitCommands(FggVkCore *core) {

	core->p_cmd_pools = (VkCommandPool*)malloc(core->queue_family_index_count * sizeof(VkCommandPool));

	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(core->queue_family_index_count * sizeof(uint32_t));
	core->p_cmd_pools[0] = fggCreateCmdPool(core->device, core->graphics_queue_index);
	if (core->queue_family_index_count == 2) {
		core->p_cmd_pools[1] = fggCreateCmdPool(core->device, core->present_queue_index);
	}
	free(pQueueFamilyIndices);

	core->p_cmd_buffers = (VkCommandBuffer*)malloc(core->queue_family_index_count * sizeof(VkCommandBuffer));
	for (uint32_t i = 0; i < core->queue_family_index_count; i++) {
		core->p_cmd_buffers[i] = fggCreateCmdBuffer(core->device, core->p_cmd_pools[i]);;
	}
}

VkCommandPool fggCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex) {

	VkCommandPoolCreateInfo cmdPoolCreateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,			//sType;
		NULL,												//pNext;
		VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,	//flags;
		queueFamilyIndex									//queueFamilyIndex;
	};


#ifndef NDEBUG
	puts("creating command pool");
#endif

	VkCommandPool cmdPool = 0;
	fggCheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

VkCommandBuffer fggCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool) {
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,	//sType;
		NULL,											//pNext;
		cmdPool,										//commandPool;
		VK_COMMAND_BUFFER_LEVEL_PRIMARY,				//level;
		1												//commandBufferCount;
	};

#ifndef NDEBUG
	puts("creating command buffer");
#endif

	VkCommandBuffer cmdBuffer;
	fggCheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), 
		"error creating command buffer"
	);

	return cmdBuffer;
}

void fggCreateRenderPass(FggVkCore* core) {
	
	VkAttachmentDescription colorAttachmentDescription = {
		0,									//flags;
		core->image_format,					//format;
		1,									//samples;
		VK_ATTACHMENT_LOAD_OP_CLEAR,		//loadOp;
		VK_ATTACHMENT_STORE_OP_STORE,		//storeOp;
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,	//stencilLoadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,	//stencilStoreOp;
		VK_IMAGE_LAYOUT_UNDEFINED,			//initialLayout;
		VK_IMAGE_LAYOUT_PRESENT_SRC_KHR		//finalLayout;
	};

	VkAttachmentReference colorAttachmentReference = {
		0,
		VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL
	};

	VkSubpassDescription subpassDescription = {
		0,									//flags;
		VK_PIPELINE_BIND_POINT_GRAPHICS,	//pipelineBindPoint;
		0,									//inputAttachmentCount;
		NULL,								//pInputAttachments;
		1,									//colorAttachmentCount;
		&colorAttachmentReference,			//pColorAttachments;
		NULL,								//pResolveAttachments;
		NULL,								//pDepthStencilAttachment;
		0,									//preserveAttachmentCount;
		NULL								//pPreserveAttachments;
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		1,											//attachmentCount;
		&colorAttachmentDescription,				//pAttachments;
		1,											//subpassCount;
		&subpassDescription,						//pSubpasses;
		0,											//dependencyCount;
		NULL										//pDependencies;
	};

#ifndef NDEBUG
	puts("creating render pass");
#endif

	fggCheckVkResult(
		vkCreateRenderPass(core->device, &renderPassCreateInfo, NULL, &core->render_pass),
		"error creating render pass"
	);
}

void fggSetFramebuffers(FggVkCore* core) {
	
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		core->render_pass,							//renderPass;
		1,											//attachmentCount;
		NULL,										//pAttachments;
		core->window.width,							//width;
		core->window.height,						//height;
		1											//layers;
	};

	//NOTE: ONE FRAMEBUFFER FOR EACH ATTACHMENT

	core->p_frame_buffers = (VkFramebuffer*)malloc(core->swapchain_image_count * sizeof(VkFramebuffer));

	for (uint32_t i = 0; i < core->swapchain_image_count; i++) {
		framebufferCreateInfo.pAttachments = &core->p_swapchain_image_views[i];
		fggCheckVkResult(
			vkCreateFramebuffer(core->device, &framebufferCreateInfo, NULL, &core->p_frame_buffers[i]),
			"error creating framebuffer"
		);
	}
}

void fggSetSyncObjects(FggVkCore* core) {
	VkFenceCreateInfo render_fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	fggCheckVkResult(
		vkCreateFence(core->device, &render_fenceCreateInfo, NULL, &core->render_fence),
		"error creating fence"
	);

	VkSemaphoreCreateInfo semaphoreCreateInfo = {
		VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0											//flags;
	};

#ifndef NDEBUG 
	puts("creating sync objects");
#endif

	fggCheckVkResult(
		vkCreateSemaphore(core->device, &semaphoreCreateInfo, NULL, &core->render_semaphore),
		"error creating render semaphore"
	);
	fggCheckVkResult(
		vkCreateSemaphore(core->device, &semaphoreCreateInfo, NULL, &core->present_semaphore),
		"error creating present semaphore"
	);
}

void fggSurfaceRelease(FggVkCore* core) {
	
	vkDeviceWaitIdle(core->device);

	for (uint32_t i = 0; i < core->swapchain_image_count; i++) {
		vkDestroyFramebuffer(core->device, core->p_frame_buffers[i], NULL);
		vkDestroyImageView(core->device, core->p_swapchain_image_views[i], NULL);
	}
	vkDestroySwapchainKHR(core->device, core->swapchain, NULL);

	core->swapchain_image_count = 0;
	free(core->p_frame_buffers);
	free(core->p_swapchain_image_views);
	free(core->p_swapchain_images);

	vkDestroySurfaceKHR(core->instance, core->surface, NULL);
}

void fggCmdRelease(FggVkCore* core) {

	vkDeviceWaitIdle(core->device);

	vkDestroySemaphore(core->device, core->present_semaphore, NULL);
	vkDestroySemaphore(core->device, core->render_semaphore, NULL);
	vkDestroyFence(core->device, core->render_fence, NULL);

	vkFreeCommandBuffers(core->device, core->p_cmd_pools[0], 1, &core->p_cmd_buffers[0]);
	vkDestroyCommandPool(core->device, core->p_cmd_pools[0], NULL);
	if (core->present_queue_index != core->graphics_queue_index) {
		vkFreeCommandBuffers(core->device, core->p_cmd_pools[1], 1, &core->p_cmd_buffers[1]);
		vkDestroyCommandPool(core->device, core->p_cmd_pools[1], NULL);
	}

	free(core->p_cmd_buffers);
	free(core->p_cmd_pools); 

	core->queue_family_index_count = 0;
}

void fggCoreRelease(FggVkCore* core) {	
	
	vkDeviceWaitIdle(core->device);

	vkDestroyRenderPass(core->device, core->render_pass, NULL);
	vkDestroyDevice(core->device, NULL);
	vkDestroyInstance(core->instance, NULL);
}