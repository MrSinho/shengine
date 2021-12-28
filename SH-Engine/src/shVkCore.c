#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "shVkCore.h"
#include "shWindow.h"
#include "shUtilities.h"
#include "shComponents.h"

#ifdef _MSC_VER
#pragma warning (disable: 6011)
#pragma warning (disable: 6385)
#pragma warning (disable: 6386)
#pragma warning (disable: 6255)
#endif // _MSC_VER

/*
* First setup
*/

ShVkCore shVkCoreInitPrerequisites(uint32_t width, uint32_t height, const char *title) {
	
	ShWindow window = {
		NULL,
		width, 
		height, 
		title
	};

	ShVkCore core = { 0 };
	core.required_queue_flag = VK_QUEUE_GRAPHICS_BIT;
	core.swapchain_image_format = SH_SWAPCHAIN_IMAGE_FORMAT;
	core.window = window;

	shInitGLFW(&core.window);
	
	return core;
}

void shCreateInstance(ShVkCore* p_core) {

	VkApplicationInfo application_info = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,	//sType;
		NULL,								//pNext;
		"VK-Compute Application",			//pApplicationName;
		VK_MAKE_VERSION(0, 1, 0),			//applicationVersion;
		"VK-Compute Engine",				//pEngineName;
		VK_MAKE_VERSION(0, 1, 0),			//engineVersion;
		SH_VULKAN_VERSION,					//apiVersion;
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
		if (shCheckValidationLayer(khronos_validation)) {
			instance_create_info.enabledLayerCount = 1;
			instance_create_info.ppEnabledLayerNames = &khronos_validation;
		}

		puts("creating vkinstance");
#endif

	shCheckVkResult(
		vkCreateInstance(&instance_create_info, VK_NULL_HANDLE, &p_core->instance),
		"error creating vkinstance"
	);
}

void shCreateWindowSurface(ShVkCore* p_core) {
	
	shCheckVkResult(
		glfwCreateWindowSurface(p_core->instance, p_core->window.window, NULL, &p_core->surface),
		"error creating window surface"
	);
}

extern VkSurfaceCapabilitiesKHR shGetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &surfaceCapabilities);
	return surfaceCapabilities;
}

void shSetPhysicalDevice(ShVkCore* p_core) {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, NULL);

	VkPhysicalDevice *pDevices = (VkPhysicalDevice*)malloc(pDeviceCount*sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(p_core->instance, &pDeviceCount, pDevices);

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
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, p_core->surface, &surfaceSupport);
				if (surfaceSupport) {
					surfaceQueueFamilyIndices[i] = j;
				}
			}

			// GRAPHICS QUEUE
			if (pQueueFamilyProperties[j].queueFlags & p_core->required_queue_flag) {
				graphicsQueueFamilyIndices[i] = j;
			}

			//SUITABLE
			if (pQueueFamilyProperties[j].queueFlags & p_core->required_queue_flag && surfaceSupport) {
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

		VkPhysicalDeviceFeatures deviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevices[i], &deviceFeatures);
		
		scores[i] += pDeviceProperties.limits.maxComputeSharedMemorySize;
		scores[i] += pDeviceProperties.limits.maxComputeWorkGroupInvocations;

	}

	if (suitableDeviceCount > 1) {

		for (uint32_t i = 1; i < suitableDeviceCount; i++) {
			if (scores[i] > scores[i - 1]) {
				p_core->physical_device = pDevices[i];
				p_core->graphics_queue_index = graphicsQueueFamilyIndices[i];
				p_core->present_queue_index = surfaceQueueFamilyIndices[i];
			}
			else {
				p_core->physical_device = pDevices[i - 1 ];
				p_core->graphics_queue_index = graphicsQueueFamilyIndices[i - 1];
				p_core->present_queue_index = surfaceQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		p_core->physical_device = pDevices[0];
		p_core->graphics_queue_index = graphicsQueueFamilyIndices[0];
		p_core->present_queue_index = surfaceQueueFamilyIndices[0];
	}
	
	if (p_core->graphics_queue_index == p_core->present_queue_index) {
		p_core->queue_family_index_count = 1;
	}
	else {
		p_core->queue_family_index_count = 2;
	}
	free(scores);
	free(graphicsQueueFamilyIndices);
	free(surfaceQueueFamilyIndices);
	free(pDevices);

	vkGetPhysicalDeviceProperties(p_core->physical_device, &p_core->physical_device_properties);

#ifndef NDEBUG
		printf("using %s \n", p_core->physical_device_properties.deviceName);
#endif

}



/*
*	Logical device creation + command buffers
*/

VkDeviceQueueCreateInfo shSetQueueInfo(const uint32_t queueFamilyIndex, const float *priority) {
	
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

void shSetLogicalDevice(ShVkCore* p_core) {
	
	const float queue_priority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueInfo = shSetQueueInfo(p_core->graphics_queue_index, &queue_priority);

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

	shCheckVkResult(
		vkCreateDevice(p_core->physical_device, &deviceCreateInfo, NULL, &p_core->device),
		"error creating logical device"
	);
		
}

void shGetGraphicsQueue(ShVkCore* p_core) {
	vkGetDeviceQueue(p_core->device, p_core->graphics_queue_index, 0, &p_core->graphics_queue);
}

void shInitSwapchainData(ShVkCore* p_core) {
	shCreateSwapchain(p_core);
	shGetSwapchainImages(p_core);
	shCreateSwapchainImageViews(p_core);
}

void shCreateDepthImageView(ShVkCore* p_core) {
	shCreateImageView(p_core, p_core->depth_image, SH_DEPTH_IMAGE, &p_core->depth_image_view);
}

void shCreateSwapchain(ShVkCore* p_core) {
	// Get the list of VkFormats that are supported:
    uint32_t formatCount;
    int res = vkGetPhysicalDeviceSurfaceFormatsKHR(p_core->physical_device, p_core->surface, &formatCount, NULL);
    assert(res == VK_SUCCESS);
    VkSurfaceFormatKHR *surfFormats = (VkSurfaceFormatKHR *)malloc(formatCount * sizeof(VkSurfaceFormatKHR));
    res = vkGetPhysicalDeviceSurfaceFormatsKHR(p_core->physical_device, p_core->surface, &formatCount, surfFormats);
    assert(res == VK_SUCCESS);
    // If the format list includes just one entry of VK_FORMAT_UNDEFINED,
    // the surface has no preferred format.  Otherwise, at least one
    // supported format will be returned.
    if (formatCount == 1 && surfFormats[0].format == VK_FORMAT_UNDEFINED) {
        p_core->swapchain_image_format = VK_FORMAT_B8G8R8A8_UNORM;
    } else {
        assert(formatCount >= 1);
        p_core->swapchain_image_format = surfFormats[0].format;
    }
    free(surfFormats);

    VkSurfaceCapabilitiesKHR surfCapabilities;

    res = vkGetPhysicalDeviceSurfaceCapabilitiesKHR(p_core->physical_device, p_core->surface, &surfCapabilities);
    assert(res == VK_SUCCESS);

    uint32_t presentModeCount;
    res = vkGetPhysicalDeviceSurfacePresentModesKHR(p_core->physical_device, p_core->surface, &presentModeCount, NULL);
    assert(res == VK_SUCCESS);
    //VkPresentModeKHR *presentModes = (VkPresentModeKHR *)malloc(presentModeCount * sizeof(VkPresentModeKHR));

    //res = vkGetPhysicalDeviceSurfacePresentModesKHR(p_core->physical_device, p_core->surface, &presentModeCount, presentModes);
    //assert(res == VK_SUCCESS);

    VkExtent2D swapchainExtent;
    // width and height are either both 0xFFFFFFFF, or both not 0xFFFFFFFF.
    if (surfCapabilities.currentExtent.width == 0xFFFFFFFF) {
        // If the surface size is undefined, the size is set to
        // the size of the images requested.
        swapchainExtent.width = p_core->window.width;
        swapchainExtent.height = p_core->window.height;
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
    swapchain_ci.surface = p_core->surface;
    swapchain_ci.minImageCount = desiredNumberOfSwapChainImages;
    swapchain_ci.imageFormat = p_core->swapchain_image_format;
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
    uint32_t queueFamilyIndices[2] = {(uint32_t)p_core->graphics_queue_index, (uint32_t)p_core->present_queue_index};
    if (p_core->graphics_queue_index != p_core->present_queue_index) {
        // If the graphics and present queues are from different queue families,
        // we either have to explicitly transfer ownership of images between
        // the queues, or we have to create the swapchain with imageSharingMode
        // as VK_SHARING_MODE_CONCURRENT
        swapchain_ci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        swapchain_ci.queueFamilyIndexCount = 2;
        swapchain_ci.pQueueFamilyIndices = queueFamilyIndices;
    }

	shCheckVkResult(
		vkCreateSwapchainKHR(p_core->device, &swapchain_ci, NULL, &p_core->swapchain),
		"error creating swapchain"
	);

}

void shGetSwapchainImages(ShVkCore* p_core) {
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, NULL);
	p_core->p_swapchain_images = (VkImage*)malloc(p_core->swapchain_image_count * sizeof(VkImage));
	vkGetSwapchainImagesKHR(p_core->device, p_core->swapchain, &p_core->swapchain_image_count, p_core->p_swapchain_images);
}

void shCreateImageView(ShVkCore* p_core, const VkImage image, const shImageType type, VkImageView* p_image_view) {
	VkImageViewCreateInfo imageViewCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	//sType;
			NULL,										//pNext;
			0,											//flags;
			image,										//image;
			VK_IMAGE_VIEW_TYPE_2D,						//viewType;
			0,											//format;
			VK_COMPONENT_SWIZZLE_IDENTITY,				//components;
			0
	};
	//aspectMask;
	if (type == SH_SWAPCHAIN_IMAGE) {
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;	
		imageViewCreateInfo.format = p_core->swapchain_image_format;
	}
	else if (type == SH_DEPTH_IMAGE) {
		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
		imageViewCreateInfo.format = SH_DEPTH_IMAGE_FORMAT;
	}
	imageViewCreateInfo.subresourceRange.baseMipLevel = 0;							//baseMipLevel;
	imageViewCreateInfo.subresourceRange.levelCount = 1;							//levelCount;
	imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;						//baseArrayLayer;
	imageViewCreateInfo.subresourceRange.layerCount = 1;							//layerCount;
	shCheckVkResult(
		vkCreateImageView(p_core->device, &imageViewCreateInfo, NULL, p_image_view),
		"error creating image view"
	);
}

void shCreateSwapchainImageViews(ShVkCore* p_core) {
	p_core->p_swapchain_image_views = (VkImageView*)malloc(p_core->swapchain_image_count * sizeof(VkImageView));
	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		shCreateImageView(p_core, p_core->p_swapchain_images[i], SH_SWAPCHAIN_IMAGE, &p_core->p_swapchain_image_views[i]);
	}
}

void shInitCommands(ShVkCore* p_core) {

	p_core->p_cmd_pools = (VkCommandPool*)malloc(p_core->queue_family_index_count * sizeof(VkCommandPool));

	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(p_core->queue_family_index_count * sizeof(uint32_t));
	p_core->p_cmd_pools[0] = shCreateCmdPool(p_core->device, p_core->graphics_queue_index);
	if (p_core->queue_family_index_count == 2) {
		p_core->p_cmd_pools[1] = shCreateCmdPool(p_core->device, p_core->present_queue_index);
	}
	free(pQueueFamilyIndices);

	p_core->p_cmd_buffers = (VkCommandBuffer*)malloc(p_core->queue_family_index_count * sizeof(VkCommandBuffer));
	for (uint32_t i = 0; i < p_core->queue_family_index_count; i++) {
		p_core->p_cmd_buffers[i] = shCreateCmdBuffer(p_core->device, p_core->p_cmd_pools[i]);;
	}
}

VkCommandPool shCreateCmdPool(const VkDevice device, uint32_t queueFamilyIndex) {

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
	shCheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

VkCommandBuffer shCreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool) {
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
	shCheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), 
		"error creating command buffer"
	);

	return cmdBuffer;
}

void shCreateRenderPass(ShVkCore* p_core) {
	
	VkAttachmentDescription colorAttachmentDescription = {
		0,									//flags;
		p_core->swapchain_image_format,		//format;
		VK_SAMPLE_COUNT_1_BIT,				//samples;
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

	VkAttachmentDescription depthAttachmentDescription = {
		0,													//flags;
		SH_DEPTH_IMAGE_FORMAT,								//format;
		VK_SAMPLE_COUNT_1_BIT,								//samples;
		VK_ATTACHMENT_LOAD_OP_CLEAR,						//loadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,					//storeOp;
		VK_ATTACHMENT_LOAD_OP_DONT_CARE,					//stencilLoadOp;
		VK_ATTACHMENT_STORE_OP_DONT_CARE,					//stencilStoreOp;
		VK_IMAGE_LAYOUT_UNDEFINED,							//initialLayout;
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,	//finalLayout;
	};

	VkAttachmentReference depthAttachmentReference = {
		1, 													//attachment
		VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL	//layout
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
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		subpassDescription.pDepthStencilAttachment = &depthAttachmentReference;
	}

	VkAttachmentDescription attachment_descriptions[2] = {
		colorAttachmentDescription, depthAttachmentDescription
	};

	VkSubpassDependency subpassDependency = {
		0,																							//srcSubpass;
		0,																							//dstSubpass;
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,	//srcStageMask;
		VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,	//dstStageMask;
		0, 																							//srcAccessMask;
		VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,		//dstAccessMask;
		VK_DEPENDENCY_BY_REGION_BIT																	//dependencyFlags;
	};

	VkRenderPassCreateInfo renderPassCreateInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		1,											//attachmentCount;
		attachment_descriptions,					//pAttachments;
		1,											//subpassCount;
		&subpassDescription,						//pSubpasses;
		0,											//dependencyCount;
		NULL										//pDependencies;
	};
	if (p_core->depth_image_view != VK_NULL_HANDLE) {
		renderPassCreateInfo.attachmentCount = 2;
		renderPassCreateInfo.dependencyCount = 1;
		renderPassCreateInfo.pDependencies = &subpassDependency;
	}

#ifndef NDEBUG
	puts("creating render pass");
#endif

	shCheckVkResult(
		vkCreateRenderPass(p_core->device, &renderPassCreateInfo, NULL, &p_core->render_pass),
		"error creating render pass"
	);
}

void shSetFramebuffers(ShVkCore* p_core) {
	
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		p_core->render_pass,						//renderPass;
		1,											//attachmentCount;
		NULL,										//pAttachments;
		p_core->window.width,						//width;
		p_core->window.height,						//height;
		1											//layers;
	};

	//NOTE: ONE FRAMEBUFFER FOR EACH ATTACHMENT

	p_core->p_frame_buffers = (VkFramebuffer*)malloc(p_core->swapchain_image_count * sizeof(VkFramebuffer));

	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		VkImageView attachments[2] = {
			p_core->p_swapchain_image_views[i], p_core->depth_image_view 
		};
		if (p_core->depth_image_view != VK_NULL_HANDLE) {
			framebufferCreateInfo.attachmentCount = 2;
		}
		framebufferCreateInfo.pAttachments = attachments;
		shCheckVkResult(
			vkCreateFramebuffer(p_core->device, &framebufferCreateInfo, NULL, &p_core->p_frame_buffers[i]),
			"error creating framebuffer"
		);
	}
}

void shSetSyncObjects(ShVkCore* p_core) {
	VkFenceCreateInfo render_fenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	shCheckVkResult(
		vkCreateFence(p_core->device, &render_fenceCreateInfo, NULL, &p_core->render_fence),
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

	shCheckVkResult(
		vkCreateSemaphore(p_core->device, &semaphoreCreateInfo, NULL, &p_core->render_semaphore),
		"error creating render semaphore"
	);
	shCheckVkResult(
		vkCreateSemaphore(p_core->device, &semaphoreCreateInfo, NULL, &p_core->present_semaphore),
		"error creating present semaphore"
	);
}

void shSwapchainRelease(ShVkCore* p_core) {
	vkDeviceWaitIdle(p_core->device);

	for (uint32_t i = 0; i < p_core->swapchain_image_count; i++) {
		vkDestroyFramebuffer(p_core->device, p_core->p_frame_buffers[i], NULL);
		vkDestroyImageView(p_core->device, p_core->p_swapchain_image_views[i], NULL);
	}
	vkDestroySwapchainKHR(p_core->device, p_core->swapchain, NULL);
}

void shDepthBufferRelease(ShVkCore* p_core) {
	vkDestroyImageView(p_core->device, p_core->depth_image_view, NULL);
	vkDestroyImage(p_core->device, p_core->depth_image, NULL);
	vkFreeMemory(p_core->device, p_core->depth_image_memory, NULL);
}

void shSurfaceRelease(ShVkCore* p_core) {
	vkDeviceWaitIdle(p_core->device);

	p_core->swapchain_image_count = 0;
	free(p_core->p_frame_buffers);
	free(p_core->p_swapchain_image_views);
	free(p_core->p_swapchain_images);

	vkDestroySurfaceKHR(p_core->instance, p_core->surface, NULL);
}

void shCmdRelease(ShVkCore* p_core) {

	vkDeviceWaitIdle(p_core->device);

	vkDestroySemaphore(p_core->device, p_core->present_semaphore, NULL);
	vkDestroySemaphore(p_core->device, p_core->render_semaphore, NULL);
	vkDestroyFence(p_core->device, p_core->render_fence, NULL);

	vkFreeCommandBuffers(p_core->device, p_core->p_cmd_pools[0], 1, &p_core->p_cmd_buffers[0]);
	vkDestroyCommandPool(p_core->device, p_core->p_cmd_pools[0], NULL);
	if (p_core->present_queue_index != p_core->graphics_queue_index) {
		vkFreeCommandBuffers(p_core->device, p_core->p_cmd_pools[1], 1, &p_core->p_cmd_buffers[1]);
		vkDestroyCommandPool(p_core->device, p_core->p_cmd_pools[1], NULL);
	}

	free(p_core->p_cmd_buffers);
	free(p_core->p_cmd_pools); 

	p_core->queue_family_index_count = 0;
}

void shRenderPassRelease(ShVkCore* p_core) {
	vkDeviceWaitIdle(p_core->device);
	vkDestroyRenderPass(p_core->device, p_core->render_pass, NULL);
}

void shDeviceRelease(ShVkCore* p_core) {
	vkDeviceWaitIdle(p_core->device);
	vkDestroyDevice(p_core->device, NULL);
}

void shInstanceRelease(ShVkCore* p_core) {	
	vkDestroyInstance(p_core->instance, NULL);
}

void shVulkanRelease(ShVkCore* p_core) {
	shSwapchainRelease(p_core);
	shDepthBufferRelease(p_core);
	shSurfaceRelease(p_core);
	shCmdRelease(p_core);
	shRenderPassRelease(p_core);
	shDeviceRelease(p_core);
	shInstanceRelease(p_core);
}