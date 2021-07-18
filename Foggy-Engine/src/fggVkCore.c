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
		VK_NULL_HANDLE,				//instance;
		VK_NULL_HANDLE,				//physicalDevice;
		VK_NULL_HANDLE,				//device;
		{NULL,
		width, 
		height, 
		title},						//window;
		VK_NULL_HANDLE,				//surface;
		VK_QUEUE_GRAPHICS_BIT,		//requiredQueueFlag;
		1,							//queueFamilyIndexCount
		0,							//graphicsQueueIndex;
		0,							//presentQueueIndex;
		VK_NULL_HANDLE,				//graphicsQueue;
		VK_NULL_HANDLE,				//pCmdPools;
		VK_NULL_HANDLE,				//pCmdBuffers;
		VK_NULL_HANDLE,				//swapchain;
		VK_FORMAT_R8G8B8A8_UNORM,	//imageFormat;
		0,							//swapchainImageCount;
		VK_NULL_HANDLE,				//pSwapchainImages;
		0,							//pSwapchainImageViews;
		VK_NULL_HANDLE,				//pFramebuffers;
		VK_NULL_HANDLE,				//renderPass;
		VK_NULL_HANDLE,				//renderSemaphore;
		VK_NULL_HANDLE,				//presentSemaphore;
		VK_NULL_HANDLE,				//renderFence;
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

	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,	//sType;
		NULL,								//pNext;
		"VK-Compute Application",			//pApplicationName;
		VK_MAKE_VERSION(0, 1, 0),			//applicationVersion;
		"VK-Compute Engine",				//pEngineName;
		VK_MAKE_VERSION(0, 1, 0),			//engineVersion;
		FGG_VULKAN_VERSION,					//apiVersion;
	};

	const char* khronos_validation = "VK_LAYER_KHRONOS_validation";

	uint32_t instanceExtensionsCount;
	const char** extensionNames = glfwGetRequiredInstanceExtensions(&instanceExtensionsCount);

	VkInstanceCreateInfo instanceCreateInfo = {
		VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		&applicationInfo,						//pApplicationInfo;
		0,										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		instanceExtensionsCount,				//enabledExtensionCount;
		& extensionNames[0]						//ppEnabledExtensionNames;
	};

#ifndef NDEBUG
		if (fggCheckValidationLayer(khronos_validation)) {
			instanceCreateInfo.enabledLayerCount = 1;
			instanceCreateInfo.ppEnabledLayerNames = &khronos_validation;
		}

		puts("creating vkinstance");
#endif

	fggCheckVkResult(
		vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &core->instance),
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
			if (pQueueFamilyProperties[j].queueFlags & core->requiredQueueFlag) {
				graphicsQueueFamilyIndices[i] = j;
			}

			//SUITABLE
			if (pQueueFamilyProperties[j].queueFlags & core->requiredQueueFlag && surfaceSupport) {
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
				core->physicalDevice = pDevices[i];
				core->graphicsQueueIndex = graphicsQueueFamilyIndices[i];
				core->presentQueueIndex = surfaceQueueFamilyIndices[i];
			}
			else {
				core->physicalDevice = pDevices[i - 1 ];
				core->graphicsQueueIndex = graphicsQueueFamilyIndices[i - 1];
				core->presentQueueIndex = surfaceQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		core->physicalDevice = pDevices[0];
		core->graphicsQueueIndex = graphicsQueueFamilyIndices[0];
		core->presentQueueIndex = surfaceQueueFamilyIndices[0];
	}
	
	if (core->graphicsQueueIndex == core->presentQueueIndex) {
		core->queueFamilyIndexCount = 1;
	}
	else {
		core->queueFamilyIndexCount = 2;
	}
	free(scores);
	free(graphicsQueueFamilyIndices);
	free(surfaceQueueFamilyIndices);
	free(pDevices);

#ifndef NDEBUG
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(core->physicalDevice, &physicalDeviceProperties);
		printf("using %s \n", physicalDeviceProperties.deviceName);
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
	
	const float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueInfo = fggSetQueueInfo(core->graphicsQueueIndex, &queuePriority);

	const char* swapchainExtensionName = VK_KHR_SWAPCHAIN_EXTENSION_NAME;

	VkDeviceCreateInfo deviceCreateInfo = {
		VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		1, 										//queueCreateInfoCount;
		&graphicsQueueInfo,						//pQueueCreateInfos;
		0, 										//enabledLayerCount;
		NULL,									//ppEnabledLayerNames;
		1, 										//enabledExtensionCount;
		&swapchainExtensionName,							//ppEnabledExtensionNames;
		NULL									//pEnabledFeatures;
	};

#ifndef NDEBUG
	puts("creating logical device");
#endif

	fggCheckVkResult(
		vkCreateDevice(core->physicalDevice, &deviceCreateInfo, NULL, &core->device),
		"error creating logical device"
	);
		
}

void fggGetGraphicsQueue(FggVkCore *core) {
	vkGetDeviceQueue(core->device, core->graphicsQueueIndex, 0, &core->graphicsQueue);
}

void fggInitSwapchainData(FggVkCore *core) {
	fggCreateSwapchain(core);
	fggGetSwapchainImages(core);
	fggCreateSwapchainImageViews(core);
}

void fggCreateSwapchain(FggVkCore* core) {
	
	VkSurfaceCapabilitiesKHR sCapabilities = fggGetSurfaceCapabilities(core->physicalDevice, core->surface);

	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(core->physicalDevice, core->surface, &surfaceFormatCount, NULL);
	VkSurfaceFormatKHR* pSurfaceFormats = (VkSurfaceFormatKHR*)malloc(surfaceFormatCount * sizeof(VkSurfaceFormatKHR));
	vkGetPhysicalDeviceSurfaceFormatsKHR(core->physicalDevice, core->surface, &surfaceFormatCount, pSurfaceFormats);

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
		1,												//queueFamilyIndexCount;
		&core->graphicsQueueIndex,						//pQueueFamilyIndices;
		VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR,			//preTransform;
		VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,				//compositeAlpha;
		VK_PRESENT_MODE_FIFO_KHR,						//presentMode;
		1,												//clipped;
		NULL,											//oldSwapchain;
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
		if (core->graphicsQueueIndex != core->presentQueueIndex) {
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			pQueueFamilyIndices[0] = core->graphicsQueueIndex; 
			pQueueFamilyIndices[1] = core->presentQueueIndex;
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
}

void fggGetSwapchainImages(FggVkCore *core) {
	vkGetSwapchainImagesKHR(core->device, core->swapchain, &core->swapchainImageCount, NULL);
	core->pSwapchainImages = (VkImage*)malloc(core->swapchainImageCount * sizeof(VkImage));
	vkGetSwapchainImagesKHR(core->device, core->swapchain, &core->swapchainImageCount, core->pSwapchainImages);
}

void fggCreateSwapchainImageViews(FggVkCore *core) {
	core->pSwapchainImageViews = (VkImageView*)malloc(core->swapchainImageCount * sizeof(VkImageView));

	for (uint32_t i = 0; i < core->swapchainImageCount; i++) {
		
		VkImageViewCreateInfo imageViewCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	//sType;
			NULL,										//pNext;
			0,											//flags;
			core->pSwapchainImages[i],					//image;
			VK_IMAGE_VIEW_TYPE_2D,						//viewType;
			core->imageFormat,							//format;
			VK_COMPONENT_SWIZZLE_IDENTITY,				//components;
			0
		};

		imageViewCreateInfo.subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;	//aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;							//baseMipLevel;
		imageViewCreateInfo.subresourceRange.levelCount = 1;							//levelCount;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;						//baseArrayLayer;
		imageViewCreateInfo.subresourceRange.layerCount = 1;							//layerCount;

		fggCheckVkResult(
			vkCreateImageView(core->device, &imageViewCreateInfo, NULL, &core->pSwapchainImageViews[i]),
			"error creating image view"
		);
	}
}

void fggInitCommands(FggVkCore *core) {

	core->pCmdPools = (VkCommandPool*)malloc(core->queueFamilyIndexCount * sizeof(VkCommandPool));

	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(core->queueFamilyIndexCount * sizeof(uint32_t));
	core->pCmdPools[0] = fggCreateCmdPool(core->device, core->graphicsQueueIndex);
	if (core->queueFamilyIndexCount == 2) {
		core->pCmdPools[1] = fggCreateCmdPool(core->device, core->presentQueueIndex);
	}
	free(pQueueFamilyIndices);

	core->pCmdBuffers = (VkCommandBuffer*)malloc(core->queueFamilyIndexCount * sizeof(VkCommandBuffer));
	for (uint32_t i = 0; i < core->queueFamilyIndexCount; i++) {
		core->pCmdBuffers[i] = fggCreateCmdBuffer(core->device, core->pCmdPools[i]);;
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

	VkCommandPool cmdPool = NULL;
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
		core->imageFormat,					//format;
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
		vkCreateRenderPass(core->device, &renderPassCreateInfo, NULL, &core->renderPass),
		"error creating render pass"
	);
}

void fggSetFramebuffers(FggVkCore* core) {
	
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		core->renderPass,							//renderPass;
		1,											//attachmentCount;
		NULL,										//pAttachments;
		core->window.width,							//width;
		core->window.height,						//height;
		1											//layers;
	};

	//NOTE: ONE FRAMEBUFFER FOR EACH ATTACHMENT

	core->pFramebuffers = (VkFramebuffer*)malloc(core->swapchainImageCount * sizeof(VkFramebuffer));

	for (uint32_t i = 0; i < core->swapchainImageCount; i++) {
		framebufferCreateInfo.pAttachments = &core->pSwapchainImageViews[i];
		fggCheckVkResult(
			vkCreateFramebuffer(core->device, &framebufferCreateInfo, NULL, &core->pFramebuffers[i]),
			"error creating framebuffer"
		);
	}
}

void fggSetSyncObjects(FggVkCore* core) {
	VkFenceCreateInfo renderFenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	fggCheckVkResult(
		vkCreateFence(core->device, &renderFenceCreateInfo, NULL, &core->renderFence),
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
		vkCreateSemaphore(core->device, &semaphoreCreateInfo, NULL, &core->renderSemaphore),
		"error creating render semaphore"
	);
	fggCheckVkResult(
		vkCreateSemaphore(core->device, &semaphoreCreateInfo, NULL, &core->presentSemaphore),
		"error creating present semaphore"
	);
}

void fggSurfaceRelease(FggVkCore* core) {
	
	vkDeviceWaitIdle(core->device);

	for (uint32_t i = 0; i < core->swapchainImageCount; i++) {
		vkDestroyFramebuffer(core->device, core->pFramebuffers[i], NULL);
		vkDestroyImageView(core->device, core->pSwapchainImageViews[i], NULL);
	}
	vkDestroySwapchainKHR(core->device, core->swapchain, NULL);

	core->swapchainImageCount = 0;
	free(core->pFramebuffers);
	free(core->pSwapchainImageViews);
	free(core->pSwapchainImages);

	vkDestroySurfaceKHR(core->instance, core->surface, NULL);
}

void fggCmdRelease(FggVkCore* core) {

	vkDeviceWaitIdle(core->device);

	vkDestroySemaphore(core->device, core->presentSemaphore, NULL);
	vkDestroySemaphore(core->device, core->renderSemaphore, NULL);
	vkDestroyFence(core->device, core->renderFence, NULL);

	vkDestroyCommandPool(core->device, core->pCmdPools[0], NULL);
	if (core->presentQueueIndex != core->graphicsQueueIndex) {
		vkDestroyCommandPool(core->device, core->pCmdPools[1], NULL);
	}

	free(core->pCmdBuffers);
	free(core->pCmdPools);

	core->queueFamilyIndexCount = 0;
}

void fggCoreRelease(FggVkCore* core) {	
	
	vkDeviceWaitIdle(core->device);

	vkDestroyRenderPass(core->device, core->renderPass, NULL);
	vkDestroyDevice(core->device, NULL);
	vkDestroyInstance(core->instance, NULL);
}