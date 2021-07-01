#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "VkDataHandler.h"
#include "VkPipelineData.h"
#include "Window.h"
#include "Utilities.h"

#pragma warning (disable: 6011)
#pragma warning (disable: 6385)
#pragma warning (disable: 6386)

/*
* First setup
*/

VkData VKDataInitPrerequisites(uint32_t width, uint32_t height, const char *title) {
	
	VkData data = {	
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

	InitGLFW(&data.window);
	
	return data;
}

void InitVulkan(VkData *data) {
	CreateInstance(data);
	CreateWindowSurface(data->instance, data->window.window, &data->surface);
	SetPhysicalDevice(data);
	SetLogicalDevice(data);
	GetGraphicsQueue(data);
}

void CreateInstance(VkData* data) {

	VkApplicationInfo applicationInfo = {
		VK_STRUCTURE_TYPE_APPLICATION_INFO,
		NULL,
		"VK-Compute Application",
		VK_MAKE_VERSION(0, 1, 0),
		"VK-Compute Engine",
		VK_MAKE_VERSION(0, 1, 0),
		VK_API_VERSION_1_2,
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
		if (CheckValidationLayer(khronos_validation)) {
			instanceCreateInfo.enabledLayerCount = 1;
			instanceCreateInfo.ppEnabledLayerNames = &khronos_validation;
		}

		puts("creating vkinstance");
#endif

	CheckVkResult(
		vkCreateInstance(&instanceCreateInfo, VK_NULL_HANDLE, &data->instance),
		"error creating vkinstance"
	);
}

void CreateWindowSurface(const VkInstance instance, GLFWwindow *window, VkSurfaceKHR *surface) {
	
	CheckVkResult(
		glfwCreateWindowSurface(instance, window, NULL, surface),
		"error creating window surface"
	);
}

extern VkSurfaceCapabilitiesKHR GetSurfaceCapabilities(const VkPhysicalDevice pDevice, const VkSurfaceKHR surface) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities = {0};
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(pDevice, surface, &surfaceCapabilities);
	return surfaceCapabilities;
}

void SetPhysicalDevice(VkData* data) {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(data->instance, &pDeviceCount, NULL);

	VkPhysicalDevice *pDevices = (VkPhysicalDevice*)malloc(pDeviceCount*sizeof(VkPhysicalDevice));
	vkEnumeratePhysicalDevices(data->instance, &pDeviceCount, pDevices);

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
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevices[i], j, data->surface, &surfaceSupport);
				if (surfaceSupport) {
					surfaceQueueFamilyIndices[i] = j;
				}
			}

			// GRAPHICS QUEUE
			if (pQueueFamilyProperties[j].queueFlags & data->requiredQueueFlag) {
				graphicsQueueFamilyIndices[i] = j;
			}

			//SUITABLE
			if (pQueueFamilyProperties[j].queueFlags & data->requiredQueueFlag && surfaceSupport) {
				suitableDeviceCount += 1;
				break;
			}
		}
	}

	if (suitableDeviceCount == 0) {
		puts("No suitable device has been found");
		exit(-1);
	}

	uint32_t *scores = calloc(suitableDeviceCount, sizeof(uint32_t));

	for (uint32_t i = 0; i < suitableDeviceCount; i++) {

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
				data->physicalDevice = pDevices[i];
				data->graphicsQueueIndex = graphicsQueueFamilyIndices[i];
				data->presentQueueIndex = surfaceQueueFamilyIndices[i];
			}
			else {
				data->physicalDevice = pDevices[i - 1 ];
				data->graphicsQueueIndex = graphicsQueueFamilyIndices[i - 1];
				data->presentQueueIndex = surfaceQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		data->physicalDevice = pDevices[0];
		data->graphicsQueueIndex = graphicsQueueFamilyIndices[0];
		data->presentQueueIndex = surfaceQueueFamilyIndices[0];
	}
	
	if (data->graphicsQueueIndex == data->presentQueueIndex) {
		data->queueFamilyIndexCount = 1;
	}
	else {
		data->queueFamilyIndexCount = 2;
	}

#ifndef NDEBUG
		VkPhysicalDeviceProperties physicalDeviceProperties;
		vkGetPhysicalDeviceProperties(data->physicalDevice, &physicalDeviceProperties);
		printf("using %s \n", physicalDeviceProperties.deviceName);
#endif

}

/*
*	Logical device creation + command buffers
*/

VkDeviceQueueCreateInfo SetQueueInfo(const uint32_t queueFamilyIndex, const float *priority) {
	
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

void SetLogicalDevice(VkData *data) {
	
	const float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo graphicsQueueInfo = SetQueueInfo(data->graphicsQueueIndex, &queuePriority);

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

	CheckVkResult(
		vkCreateDevice(data->physicalDevice, &deviceCreateInfo, NULL, &data->device),
		"error creating logical device"
	);
		
}

void GetGraphicsQueue(VkData *data) {
	vkGetDeviceQueue(data->device, data->graphicsQueueIndex, 0, &data->graphicsQueue);
}

void CreateSwapchain(VkData* data) {
	
	VkSurfaceCapabilitiesKHR sCapabilities = GetSurfaceCapabilities(data->physicalDevice, data->surface);

	uint32_t surfaceFormatCount = 0;
	vkGetPhysicalDeviceSurfaceFormatsKHR(data->physicalDevice, data->surface, &surfaceFormatCount, NULL);
	VkSurfaceFormatKHR* pSurfaceFormats = (VkSurfaceFormatKHR*)malloc(surfaceFormatCount * sizeof(VkSurfaceFormatKHR));
	vkGetPhysicalDeviceSurfaceFormatsKHR(data->physicalDevice, data->surface, &surfaceFormatCount, pSurfaceFormats);

	VkSwapchainCreateInfoKHR swapchainCreateInfo = {
		VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,	//sType;
		NULL,											//pNext;
		0,												//flags;
		data->surface,									//surface;
		sCapabilities.minImageCount,					//minImageCount;
		data->imageFormat,								//imageFormat;
		VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,				//imageColorSpace;
		sCapabilities.currentExtent,					//imageExtent;
		1,												//imageArrayLayers;
		VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,			//imageUsage;
		VK_SHARING_MODE_EXCLUSIVE,						//imageSharingMode;
		1,												//queueFamilyIndexCount;
		&data->graphicsQueueIndex,						//pQueueFamilyIndices;
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
			if (pSurfaceFormats[i].format == data->imageFormat) {
				found = 1; break;
			}
		}
		if (found == 0) {
			data->imageFormat = pSurfaceFormats[0].format;
			swapchainCreateInfo.imageFormat = data->imageFormat;
		}
	}

	//COLORSPACE CHECK
	{
		int found = 0;
		for (uint32_t i = 0; i < surfaceFormatCount; i++) {
			if (pSurfaceFormats[i].format == data->imageFormat) {
				found = 1; break;
			}
		}
		if (found == 0) {
			data->imageFormat = pSurfaceFormats[0].format;
			swapchainCreateInfo.imageFormat = data->imageFormat;
		}
	}
	
	//SHARING MODE CHECK
	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(2 * sizeof(uint32_t));
	{
		if (data->graphicsQueueIndex != data->presentQueueIndex) {
			swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
			swapchainCreateInfo.queueFamilyIndexCount = 2;
			pQueueFamilyIndices[0] = data->graphicsQueueIndex; 
			pQueueFamilyIndices[1] = data->presentQueueIndex;
			swapchainCreateInfo.pQueueFamilyIndices = pQueueFamilyIndices;
		}
	}

#ifndef NDEBUG
	puts("creating swapchain");
#endif

	CheckVkResult(
		vkCreateSwapchainKHR(data->device, &swapchainCreateInfo, NULL, &data->swapchain),
		"error creating swapchain"
	);
}

void GetSwapchainImages(VkData *data) {
	vkGetSwapchainImagesKHR(data->device, data->swapchain, &data->swapchainImageCount, NULL);
	data->pSwapchainImages = (VkImage*)malloc(data->swapchainImageCount * sizeof(VkImage));
	vkGetSwapchainImagesKHR(data->device, data->swapchain, &data->swapchainImageCount, data->pSwapchainImages);
}

void CreateSwapchainImageViews(VkData *data) {
	data->pSwapchainImageViews = (VkImageView*)malloc(data->swapchainImageCount * sizeof(VkImageView));

	for (uint32_t i = 0; i < data->swapchainImageCount; i++) {
		
		VkImageViewCreateInfo imageViewCreateInfo = {
			VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,	//sType;
			NULL,										//pNext;
			0,											//flags;
			data->pSwapchainImages[i],					//image;
			VK_IMAGE_VIEW_TYPE_2D,						//viewType;
			data->imageFormat,							//format;
			VK_COMPONENT_SWIZZLE_IDENTITY,				//components;
			0
		};

		imageViewCreateInfo.subresourceRange.aspectMask   = VK_IMAGE_ASPECT_COLOR_BIT;	//aspectMask;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;							//baseMipLevel;
		imageViewCreateInfo.subresourceRange.levelCount = 1;							//levelCount;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;						//baseArrayLayer;
		imageViewCreateInfo.subresourceRange.layerCount = 1;							//layerCount;

		CheckVkResult(
			vkCreateImageView(data->device, &imageViewCreateInfo, NULL, &data->pSwapchainImageViews[i]),
			"error creating image view"
		);
	}
}

void InitCommands(VkData *data) {

	data->pCmdPools = (VkCommandPool*)malloc(data->queueFamilyIndexCount * sizeof(VkCommandPool));

	uint32_t* pQueueFamilyIndices = (uint32_t*)malloc(data->queueFamilyIndexCount * sizeof(uint32_t));
	data->pCmdPools[0] = CreateCommandPool(data->device, data->graphicsQueueIndex);
	if (data->queueFamilyIndexCount == 2) {
		data->pCmdPools[1] = CreateCommandPool(data->device, data->presentQueueIndex);
	}

	data->pCmdBuffers = (VkCommandBuffer*)malloc(data->queueFamilyIndexCount * sizeof(VkCommandBuffer));
	for (uint32_t i = 0; i < data->queueFamilyIndexCount; i++) {
		data->pCmdBuffers[i] = CreateCmdBuffer(data->device, data->pCmdPools[i]);;
	}
}

VkCommandPool CreateCommandPool(const VkDevice device, uint32_t queueFamilyIndex) {

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
	CheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, NULL, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

VkCommandBuffer CreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool) {
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
	CheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), 
		"error creating command buffer"
	);

	return cmdBuffer;
}

void CreateRenderPass(VkData* data) {
	
	VkAttachmentDescription colorAttachmentDescription = {
		0,									//flags;
		data->imageFormat,					//format;
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

	CheckVkResult(
		vkCreateRenderPass(data->device, &renderPassCreateInfo, NULL, &data->renderPass),
		"error creating render pass"
	);
}

void SetFramebuffers(VkData* data) {
	
	VkFramebufferCreateInfo framebufferCreateInfo = {
		VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,	//sType;
		NULL,										//pNext;
		0,											//flags;
		data->renderPass,							//renderPass;
		1,											//attachmentCount;
		NULL,										//pAttachments;
		data->window.width,							//width;
		data->window.height,						//height;
		1											//layers;
	};

	//NOTE: ONE FRAMEBUFFER FOR EACH ATTACHMENT

	data->pFramebuffers = (VkFramebuffer*)malloc(data->swapchainImageCount * sizeof(VkFramebuffer));

	for (uint32_t i = 0; i < data->swapchainImageCount; i++) {
		framebufferCreateInfo.pAttachments = &data->pSwapchainImageViews[i];
		CheckVkResult(
			vkCreateFramebuffer(data->device, &framebufferCreateInfo, NULL, &data->pFramebuffers[i]),
			"error creating framebuffer"
		);
	}
}

void SetSyncObjects(VkData* data) {
	VkFenceCreateInfo renderFenceCreateInfo = {
		VK_STRUCTURE_TYPE_FENCE_CREATE_INFO,	//sType;
		NULL,									//pNext;
		VK_FENCE_CREATE_SIGNALED_BIT			//flags;
	};

	CheckVkResult(
		vkCreateFence(data->device, &renderFenceCreateInfo, NULL, &data->renderFence),
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

	CheckVkResult(
		vkCreateSemaphore(data->device, &semaphoreCreateInfo, NULL, &data->renderSemaphore),
		"error creating render semaphore"
	);
	CheckVkResult(
		vkCreateSemaphore(data->device, &semaphoreCreateInfo, NULL, &data->presentSemaphore),
		"error creating present semaphore"
	);
}

void Draw(VkData* data, PipelineData* pipeData) {

	// wait until the GPU has finished rendereing the previous frame
	vkWaitForFences(data->device, 1, &data->renderFence, 1, 1000000000);
	vkResetFences(data->device, 1, &data->renderFence);

	//reset the command buffer and start recording
	vkResetCommandBuffer(data->pCmdBuffers[0], 0);

	// request image from the swapchain
	uint32_t swapchainImageIndex;
	vkAcquireNextImageKHR(data->device, data->swapchain, 1000000000, data->presentSemaphore, NULL, &swapchainImageIndex);

	//BEGIN INFOS
	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once, before the next reset
		NULL											//pInheritanceInfo;
	};
	
	VkClearValue clearColor = { {0.01f, 0.01f, 0.01f} };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,		//sType;
		NULL,											//pNext;
		data->renderPass,								//renderPass;
		data->pFramebuffers[swapchainImageIndex],		//framebuffer;
		{												//
			{0, 0},										//
			{data->window.width, data->window.height}	//
		},												//renderArea;
		1,												//clearValueCount;
		&clearColor										//pClearValues;
	};

	//Start recording cmdbuffer
	vkBeginCommandBuffer(data->pCmdBuffers[0], &cmdBufferBeginInfo);

	//begin the render pass (cmdbuffer must be in a recording state)
	vkCmdBeginRenderPass(data->pCmdBuffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

	//bind to pipeline
	vkCmdBindPipeline(data->pCmdBuffers[0], VK_PIPELINE_BIND_POINT_GRAPHICS, pipeData->pipeline);
	
	vkCmdDraw(data->pCmdBuffers[0], 3, 1, 0, 0);

	//end operation
	vkCmdEndRenderPass(data->pCmdBuffers[0]);
	vkEndCommandBuffer(data->pCmdBuffers[0]);

	VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;

	//submit to GPU queue
	// wait for the presentsemaphore, signal to the render semaphore
	VkSubmitInfo submitInfo = {
		VK_STRUCTURE_TYPE_SUBMIT_INFO,	//sType;
		NULL,							//pNext;
		1,								//waitSemaphoreCount;
		&data->presentSemaphore,		//pWaitSemaphores;
		&waitStage,						//pWaitDstStageMask;
		1,								//commandBufferCount;
		&data->pCmdBuffers[0],			//pCommandBuffers;
		1,								//signalSemaphoreCount;
		&data->renderSemaphore,			//pSignalSemaphores;
	};

	vkQueueSubmit(data->graphicsQueue, 1, &submitInfo, data->renderFence);

	// Present the ready image
	VkPresentInfoKHR presentInfo = {
		VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,	//sType;
		NULL,								//pNext;
		1,									//waitSemaphoreCount;
		&data->renderSemaphore, 			//pWaitSemaphores;
		1,									//swapchainCount;
		&data->swapchain,					//pSwapchains;
		&swapchainImageIndex,				//pImageIndices;
		NULL								//pResults;
	};

	vkQueuePresentKHR(data->graphicsQueue, &presentInfo);

}

extern void Cleanup(VkData* data) {

}