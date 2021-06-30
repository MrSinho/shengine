#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "VkDataHandler.h"
#include "Window.h"
#include "Utilities.h"

/*
* First setup
*/

VkData VKDataInitPrerequisites(uint32_t width, uint32_t height, const char *title) {
	
	VkData data = {
		0,							//instance
		0,							//physicalDevice
		0,							//device
		VK_QUEUE_GRAPHICS_BIT,		//requiredQueueFlag
		0,							//graphicsQueueIndex
		0,							//presentQueueIndex
		{NULL, 
		width, 
		height, 
		title},						//window					
		0,							//surface
		0,							//swapchain
		VK_FORMAT_R8G8B8A8_UNORM,	//imageFormat
		0,							//imageCount
		NULL,						//pImages
		0,							//imageViewCount
		NULL,						//pImageViews
		VK_NULL_HANDLE,				//graphicsQueue
		0,							//cmdPoolCount
		NULL,						//pCmdPools
		0,							//cmdBufferCount
		NULL,						//pCmdBuffers
		0,							//renderPass
		0,							//framebufferCount
		NULL,						//framebuffers
		NULL,						//renderSemaphore
		NULL,						//presentSemaphore
		NULL,						//renderFence
		0,							//shaderModuleCount
		NULL						//pShaderModules
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
	data->swapchainImageViewCount = data->swapchainImageCount;
	data->pSwapchainImageViews = (VkImageView*)malloc(data->swapchainImageViewCount * sizeof(VkImageView));

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
			vkCreateImageView(data->device, &imageViewCreateInfo, NULL, data->pSwapchainImageViews),
			"error creating image view"
		);
	}
}

void InitCommands(VkData *data) {

	data->cmdPoolCount	= 1;
	data->pCmdPools		= (VkCommandPool*)malloc(data->cmdPoolCount * sizeof(VkCommandPool));
	data->pCmdPools[0]	= CreateCommandPool(data->device, data->graphicsQueueIndex);

	data->cmdBufferCount = 1;
	data->pCmdBuffers	 = (VkCommandBuffer*)malloc(data->cmdBufferCount * sizeof(VkCommandBuffer));
	data->pCmdBuffers[0] = CreateCmdBuffer(data->device, data->pCmdPools[0]);
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

	data->framebufferCount = data->swapchainImageViewCount;
	data->pFramebuffers = (VkFramebuffer*)malloc(data->framebufferCount * sizeof(VkFramebuffer));

	for (uint32_t i = 0; i < data->framebufferCount; i++) {
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

void Draw(VkData* data) {

	// wait until the GPU has finished rendereing the last frame
	vkWaitForFences(data->device, 1, &data->renderFence, 1, 1000000000);
	vkResetFences(data->device, 1, &data->renderFence);

	// request image from the swapchain
	uint32_t swapchainImageIndex;
	vkAcquireNextImageKHR(data->device, data->swapchain, 1000000000, data->presentSemaphore, NULL, &swapchainImageIndex);
	
	//reset the command buffer and start recording
	vkResetCommandBuffer(data->pCmdBuffers[0], 0);

	//BEGIN INFOS
	VkCommandBufferBeginInfo cmdBufferBeginInfo = {
		VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO,	//sType;
		NULL,											//pNext;
		VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT,	//flags; //submit cmd buffer once
		NULL											//pInheritanceInfo;
	};
	
	VkClearValue clearColor = { {1.0f, 0.0f, 0.0f} };
	VkRenderPassBeginInfo renderPassBeginInfo = {
		VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
		NULL,
		data->renderPass,
		data->pFramebuffers[swapchainImageIndex],
		{
			{0, 0},
			{data->window.width, data->window.height}
		},
		1,
		&clearColor
	};

	//Start recording cmdbuffer
	vkBeginCommandBuffer(data->pCmdBuffers[0], &cmdBufferBeginInfo);

	//begin the render pass (cmdbuffer must be in a recording state)
	vkCmdBeginRenderPass(data->pCmdBuffers[0], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE);

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

VkShaderModule CreateShaderModule(const VkDevice device, const char* input, const char* output) {

	Compile_glslc_Shader(input, output);
	const char *shaderBinaryCode = ReadCode(output, "rb");
	size_t shaderbSize = sizeof(shaderBinaryCode);

	VkShaderModuleCreateInfo shaderModuleCreateInfo = {
		VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,	//sType;
		NULL,											//pNext;
		0,												//flags;
		shaderbSize / sizeof(char),						//codeSize;
		(const uint32_t*)(shaderBinaryCode)				//pCode;
	};

#ifndef NDEBUG
	puts("creating shader module using binary at: ");
#endif

	VkShaderModule shaderModule;
	CheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, NULL, &shaderModule),
		"error creating shader module"
	);

	return shaderModule;
}

#ifndef NDEBUG

int CheckValidationLayer(const char* validationLayer) {
	
	uint32_t availableLayerCount = 0;
	vkEnumerateInstanceLayerProperties(&availableLayerCount, NULL);

	VkLayerProperties *availableLayersProperties = (VkLayerProperties*)malloc(availableLayerCount * sizeof(VkLayerProperties));
	vkEnumerateInstanceLayerProperties(&availableLayerCount, availableLayersProperties);
	
	if (availableLayersProperties) {
		puts("Installed validation layers: ");
		for (uint32_t i = 0; i < availableLayerCount; i++) {
			puts(availableLayersProperties[i].layerName);
			if (strcmp(availableLayersProperties[i].layerName, validationLayer) == 0) {
				return 1;
			}
		}
	}

	return 0;
}

#endif

/*
*	Utilities
*/

void CheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		printf("%s, %s \n", errormsg, TranslateVkResult(result));
#endif	
		exit(-1);
	}
}

const char* TranslateVkResult(const VkResult vkResult) {
	switch (vkResult) {
	case VK_SUCCESS:return "VK_SUCCESS";
	case VK_NOT_READY:return "VK_NOT_READY";
	case VK_TIMEOUT:return "VK_TIMEOUT";
	case VK_EVENT_SET:return "VK_EVENT_SET";
	case VK_EVENT_RESET:return "VK_EVENT_RESET";
	case VK_INCOMPLETE:return "VK_INCOMPLETE";
	case VK_ERROR_OUT_OF_HOST_MEMORY:return "VK_ERROR_OUT_OF_HOST_MEMORY";
	case VK_ERROR_OUT_OF_DEVICE_MEMORY:return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
	case VK_ERROR_INITIALIZATION_FAILED:return "VK_ERROR_INITIALIZATION_FAILED";
	case VK_ERROR_DEVICE_LOST:return "VK_ERROR_DEVICE_LOST";
	case VK_ERROR_MEMORY_MAP_FAILED:return "VK_ERROR_MEMORY_MAP_FAILED";
	case VK_ERROR_LAYER_NOT_PRESENT:return "VK_ERROR_LAYER_NOT_PRESENT";
	case VK_ERROR_EXTENSION_NOT_PRESENT:return "VK_ERROR_EXTENSION_NOT_PRESENT";
	case VK_ERROR_FEATURE_NOT_PRESENT:return "VK_ERROR_FEATURE_NOT_PRESENT";
	case VK_ERROR_INCOMPATIBLE_DRIVER:return "VK_ERROR_INCOMPATIBLE_DRIVER";
	case VK_ERROR_TOO_MANY_OBJECTS:return "VK_ERROR_TOO_MANY_OBJECTS";
	case VK_ERROR_FORMAT_NOT_SUPPORTED:return "VK_ERROR_FORMAT_NOT_SUPPORTED";
	case VK_ERROR_SURFACE_LOST_KHR:return "VK_ERROR_SURFACE_LOST_KHR";
	case VK_SUBOPTIMAL_KHR:return "VK_SUBOPTIMAL_KHR";
	case VK_ERROR_OUT_OF_DATE_KHR:return "VK_ERROR_OUT_OF_DATE_KHR";
	case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR:return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
	case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR:return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
	case VK_ERROR_VALIDATION_FAILED_EXT:return "VK_ERROR_VALIDATION_FAILED_EXT";
	case VK_RESULT_MAX_ENUM:return "VK_RESULT_MAX_ENUM";
	}
	return "unknown vkresult";
}

const char* TranslateQueueFlags(const VkQueueFlags queueFlag) {
	if (queueFlag & VK_QUEUE_GRAPHICS_BIT) { return "VK_QUEUE_GRAPHICS_BIT"; }
	if (queueFlag & VK_QUEUE_COMPUTE_BIT) { return "VK_QUEUE_COMPUTE_BIT"; }
	if (queueFlag & VK_QUEUE_TRANSFER_BIT) { return "VK_QUEUE_TRANSFER_BIT"; }
	if (queueFlag & VK_QUEUE_SPARSE_BINDING_BIT) { return "VK_QUEUE_SPARSE_BINDING_BIT"; }
	if (queueFlag & VK_QUEUE_PROTECTED_BIT) { return "VK_QUEUE_PROTECTED_BIT"; }
#ifdef VK_ENABLE_BETA_EXTENSIONS
	if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) { return "VK_QUEUE_VIDEO_DECODE_BIT_KHR"; }
#endif											
#ifdef VK_ENABLE_BETA_EXTENSIONS				
	if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) { return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR"; }
#endif											
	if (queueFlag & VK_QUEUE_FLAG_BITS_MAX_ENUM) { return "VK_QUEUE_FLAG_BITS_MAX_ENUM"; }
	return "unknown flag";
}

void Compile_glslc_Shader(const char* input, const char* output) {
	char* cmd = "glslc ";
	strncat(cmd, input, sizeof(cmd));
	strncat(cmd, "-o", sizeof(cmd));
	strncat(cmd, output, sizeof(cmd));

#ifndef NDEBUG	
	puts(cmd);
#endif
	system(cmd);
}