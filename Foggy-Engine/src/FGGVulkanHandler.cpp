#include <stdexcept>

#ifndef NDEBUG
	#include <iostream>
#endif

#include <string.h>

#include "FGGVulkanHandler.h"
#include "Utilities.h"

#pragma warning ( disable : 26812 )

/*
* First setup
*/

void InitVulkan(FGGVulkanHandler *vulkanHandler) {
	InitGLFW(&vulkanHandler->window);
	CreateInstance(vulkanHandler);
	CreateWindowSurface(vulkanHandler->instance, vulkanHandler->window, &vulkanHandler->surface);
	SetPhysicalDevice(vulkanHandler);
	SetLogicalDevice(vulkanHandler);
	CreateSwapchain(vulkanHandler);
	GetSwapchainImages(vulkanHandler);
	CreateSwapchainImageViews(vulkanHandler);
	CreateRenderPass(vulkanHandler->swapchainImageFormat, vulkanHandler->device, &vulkanHandler->renderPass);
	CreateGraphicsPipeline(vulkanHandler, &vulkanHandler->graphicsPipeline);
	CreateFramebuffers(vulkanHandler);

	CmdBufferRecordStart(*vulkanHandler);
	RenderPassStart(*vulkanHandler);
	GraphicsPipelineDraw(&vulkanHandler->cmdBuffers[0], vulkanHandler->cmdBuffers.size(), vulkanHandler->viewport, vulkanHandler->graphicsPipeline);
	RenderPassEnd(vulkanHandler->renderPass, &vulkanHandler->cmdBuffers[0], vulkanHandler->cmdBuffers.size());
	CmdBufferRecordStop(&vulkanHandler->cmdBuffers[0], vulkanHandler->cmdBuffers.size());
	CreateSemaphore(vulkanHandler->device, &vulkanHandler->imageAvailableSemaphore);
	CreateSemaphore(vulkanHandler->device, &vulkanHandler->renderFinishedSemaphore);
}

void CreateInstance(FGGVulkanHandler* vulkanHandler) {

	//APPLICATION
	VkApplicationInfo appInfo{};
	appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
	appInfo.pApplicationName = "Foggy-Editor";
	appInfo.pEngineName = "Foggy-Engine";
	appInfo.apiVersion = VK_API_VERSION_1_2;
	appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
	appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);

	//INSTANCE
	VkInstanceCreateInfo createInfo{};
	createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
	createInfo.pApplicationInfo = &appInfo;
#ifndef NDEBUG
	if (CheckValidationLayers(*vulkanHandler)) {
		createInfo.enabledLayerCount =(uint32_t)vulkanHandler->requiredValidationLayers.size();
		createInfo.ppEnabledLayerNames = &vulkanHandler->requiredValidationLayers[0];
	}
#endif
	uint32_t glfwExtensionsCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
#ifndef NDEBUG
	std::cout << "Required instance extensions: " << std::endl;
	for (uint32_t i = 0; i < glfwExtensionsCount; i++) {
		std::cout << glfwExtensions[i] << std::endl;
	}
#endif
	createInfo.enabledExtensionCount = glfwExtensionsCount;
	createInfo.ppEnabledExtensionNames = glfwExtensions;

#ifndef NDEBUG
	std::cout << "creating vkinstance" << std::endl;
#endif

	CheckVkResult(
		vkCreateInstance(&createInfo, nullptr, &vulkanHandler->instance),
		"error creating vkinstance"
	);
}

void CreateWindowSurface(const VkInstance& instance, const FGGWindow &window, VkSurfaceKHR* surface) {
	
#ifndef NDEBUG
	std::cout << "creating window surface" << std::endl;
#endif
	
	glfwCreateWindowSurface(instance, window.window, nullptr, surface);
}

void SetPhysicalDevice(FGGVulkanHandler* vulkanHandler) {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(vulkanHandler->instance, &pDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> pDevices(pDeviceCount);
	vkEnumeratePhysicalDevices(vulkanHandler->instance, &pDeviceCount, &pDevices[0]);

	if (pDeviceCount == 0) {
		throw std::runtime_error("No Vulkan compatible gpu has been found");
	}

	// GET IF DEVICE IS SUITABLE
	std::vector<VkPhysicalDevice> suitableDevices;
	std::vector<std::set<uint32_t>> suitableDevicesQueueFamilyIndices;
	std::vector<uint32_t> suitableDevicesGraphicsQueueIndices;
	std::vector<uint32_t> suitableDevicesPresentQueueIndices;

	for (const VkPhysicalDevice &pDevice : pDevices) {
#ifndef NDEBUG
		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevice, &pDeviceProperties);
		std::cout << "Found " << pDeviceProperties.deviceName << " | driver version: " << pDeviceProperties.driverVersion << std::endl;
#endif
		//Queue family properties
		uint32_t queueFamilyPropertyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyPropertyCount, nullptr);
		std::vector<VkQueueFamilyProperties> pQueueFamilyProperties(queueFamilyPropertyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyPropertyCount, &pQueueFamilyProperties[0]);
		
		std::set<uint32_t> _queueFamilyIndices;
		bool _surfaceSupport = 0;

		uint32_t _graphicsQueueIndex;
		uint32_t _presentQueueIndex;

		for (const VkQueueFlags &queueFlag : vulkanHandler->requiredQueueFlags) {
			for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
				if (pQueueFamilyProperties[i].queueFlags & queueFlag) {
					_queueFamilyIndices.insert(i);
					if (queueFlag & VK_QUEUE_GRAPHICS_BIT) { _graphicsQueueIndex = i; }
				}
				VkBool32 __surfaceSupport;
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, vulkanHandler->surface, &__surfaceSupport);
				if (static_cast<bool>(__surfaceSupport) && !_surfaceSupport) {
					_surfaceSupport = 1;
					_queueFamilyIndices.insert(i);
					_presentQueueIndex = i;
				}
			}
		}
		if (!_queueFamilyIndices.empty() && _surfaceSupport) { 
			suitableDevices.push_back(pDevice);
			suitableDevicesQueueFamilyIndices.push_back(_queueFamilyIndices);
			suitableDevicesGraphicsQueueIndices.push_back(_graphicsQueueIndex);
			suitableDevicesPresentQueueIndices.push_back(_presentQueueIndex);
		}
	}

	if (suitableDevices.empty()) {
		throw std::runtime_error("No suitable device has been found");
	}

	//SUITABLE DEVICES SCORE 
	std::vector<uint32_t> scores;
	for (const VkPhysicalDevice& pDevice : suitableDevices) {

		uint32_t score = 0;
		
		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevice, &pDeviceProperties);

		VkPhysicalDeviceFeatures pDeviceFeatures;
		vkGetPhysicalDeviceFeatures(pDevice, &pDeviceFeatures);

		score += pDeviceFeatures.geometryShader * 1000;
		score += pDeviceProperties.limits.maxClipDistances * 100;
		score += pDeviceProperties.limits.maxImageDimension2D * 2;
		score += pDeviceProperties.limits.maxViewports * 100;
		
		scores.push_back(score);
	}
	
	//VERDETTO
	if (suitableDevices.size() > 1) {
		for (uint32_t i = 1; i < scores.size(); i++) {
			if (scores[i] > scores[i - 1]) {
				vulkanHandler->physicalDevice = suitableDevices[i];
				vulkanHandler->queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[i].begin(), suitableDevicesQueueFamilyIndices[i].end());
				vulkanHandler->graphicsQueueIndex = suitableDevicesGraphicsQueueIndices[i];
				vulkanHandler->presentQueueIndex  = suitableDevicesPresentQueueIndices[i];
			}
			else {
				vulkanHandler->physicalDevice = suitableDevices[i - 1];
				vulkanHandler->queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[i - 1].begin(), suitableDevicesQueueFamilyIndices[i - 1].end());
				vulkanHandler->graphicsQueueIndex = suitableDevicesGraphicsQueueIndices[i - 1];
				vulkanHandler->presentQueueIndex  = suitableDevicesPresentQueueIndices[i - 1];
			}
		}
	}
	else {
		vulkanHandler->physicalDevice = suitableDevices[0];
		vulkanHandler->queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[0].begin(), suitableDevicesQueueFamilyIndices[0].end());
		vulkanHandler->graphicsQueueIndex = suitableDevicesGraphicsQueueIndices[0];
		vulkanHandler->presentQueueIndex = suitableDevicesPresentQueueIndices[0];
	}

#ifndef NDEBUG
	VkPhysicalDeviceProperties pDeviceProperties;
	vkGetPhysicalDeviceProperties(vulkanHandler->physicalDevice, &pDeviceProperties);
	std::cout << "Using " << pDeviceProperties.deviceName << std::endl;
#endif
}

bool CheckPhysicalDeviceExtensions(const FGGVulkanHandler &vulkanHandler, const VkPhysicalDevice& pDevice) {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
	
	std::vector<VkExtensionProperties> pDeviceExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, &pDeviceExtensions[0]);

#ifndef NDEBUG
	std::cout << "Enumerating device extensions" << std::endl;
#endif
	uint32_t requiredExtensionsCount = (uint32_t)vulkanHandler.requiredDeviceExtensionsNames.size();
	for (const VkExtensionProperties& extensionProperties : pDeviceExtensions) {
		for (const char* requiredName : vulkanHandler.requiredDeviceExtensionsNames) {
			if (strcmp(requiredName, extensionProperties.extensionName) == 0) {
#ifndef NDEBUG
				std::cout << "Required and found " << requiredName << " | " << extensionProperties.specVersion << std::endl;
#endif
				requiredExtensionsCount += -1;
			}
		}
	}
	
	return requiredExtensionsCount == 0;
}

/*
*	Logical device creation + command buffers
*/

VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex, const float &priority) {
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pQueuePriorities = &priority;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;

	return queueCreateInfo;
}

void SetLogicalDevice(FGGVulkanHandler *vulkanHandler) {
	
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (const uint32_t& index : vulkanHandler->queueFamilyIndices) {
		queueCreateInfos.push_back(CreateQueue(index, 1.0f));
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
#ifndef NDEBUG
	deviceCreateInfo.enabledLayerCount = (uint32_t)vulkanHandler->requiredValidationLayers.size();
	deviceCreateInfo.ppEnabledLayerNames = &vulkanHandler->requiredValidationLayers[0];
#endif
	deviceCreateInfo.queueCreateInfoCount = (uint32_t)queueCreateInfos.size();
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfos[0];
	VkPhysicalDeviceFeatures pDeviceFeatures{};
	deviceCreateInfo.pEnabledFeatures = &pDeviceFeatures;
	deviceCreateInfo.enabledExtensionCount = (uint32_t)vulkanHandler->requiredDeviceExtensionsNames.size();
	deviceCreateInfo.ppEnabledExtensionNames = &vulkanHandler->requiredDeviceExtensionsNames[0];

#ifndef NDEBUG
	std::cout << "Creating logical device" << std::endl;
#endif

	CheckVkResult(
		vkCreateDevice(vulkanHandler->physicalDevice, &deviceCreateInfo, nullptr, &vulkanHandler->device),
		"error creating logical device"
	);

	for (const uint32_t& index : vulkanHandler->queueFamilyIndices) {
		
		VkQueue queue;
		vkGetDeviceQueue(vulkanHandler->device, index, 0, &queue);
		vulkanHandler->queues.push_back(queue);

		VkCommandPool cmdPool = CreateCommandPool(vulkanHandler->device, index);
		VkCommandBuffer cmdBuffer = CreateCmdBuffer(vulkanHandler->device, cmdPool);
		vulkanHandler->cmdPools.push_back(cmdPool);
		vulkanHandler->cmdBuffers.push_back(cmdBuffer);
	}
}

VkCommandPool CreateCommandPool(const VkDevice &device, uint32_t queueFamilyIndex) {

	VkCommandPoolCreateInfo cmdPoolCreateInfo;
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.pNext = nullptr;
	cmdPoolCreateInfo.flags = 0;
	cmdPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;

#ifndef NDEBUG
	std::cout << "Creating " << " command pool" << std::endl;
#endif

	VkCommandPool cmdPool = nullptr;
	CheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, nullptr, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

VkCommandBuffer CreateCmdBuffer(const VkDevice &device, const VkCommandPool &cmdPool) {
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo{};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.pNext = nullptr;
	cmdBufferAllocateInfo.commandPool = cmdPool;
	cmdBufferAllocateInfo.commandBufferCount = 1;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	
#ifndef NDEBUG
	std::cout << "Creating command buffer" << std::endl;
#endif
	VkCommandBuffer cmdBuffer;
	CheckVkResult(
		vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), 
		"error creating command buffer"
	);

	return cmdBuffer;
}

/*
*	Swapchain creation + image views
*/

void CreateSwapchain(FGGVulkanHandler *vulkanHandler) {

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &surfaceCapabilities);

	uint32_t surfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &surfaceFormatsCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &surfaceFormatsCount, &surfaceFormats[0]);
	
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &presentModeCount, &presentModes[0]);

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.surface = vulkanHandler->surface;
	
	swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
	
	if (surfaceFormatsCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		vulkanHandler->swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else if (surfaceFormatsCount > 1) {
		vulkanHandler->swapchainImageFormat = surfaceFormats[0].format;
	}
	else { throw std::runtime_error("no supported image format available"); }
	swapchainCreateInfo.imageFormat = vulkanHandler->swapchainImageFormat;

	swapchainCreateInfo.imageExtent.width = surfaceCapabilities.currentExtent.width;
	swapchainCreateInfo.imageExtent.height = surfaceCapabilities.currentExtent.height;
	
	swapchainCreateInfo.preTransform = surfaceCapabilities.currentTransform;
	if (surfaceCapabilities.supportedTransforms & VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR) {
		swapchainCreateInfo.preTransform = VK_SURFACE_TRANSFORM_IDENTITY_BIT_KHR;
	}

	swapchainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.presentMode = VK_PRESENT_MODE_FIFO_KHR;
	swapchainCreateInfo.clipped = 1;
	swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
	if (vulkanHandler->queueFamilyIndices.size() == 1) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	swapchainCreateInfo.queueFamilyIndexCount = (uint32_t)vulkanHandler->queueFamilyIndices.size();
	swapchainCreateInfo.pQueueFamilyIndices = &vulkanHandler->queueFamilyIndices[0];

#ifndef NDEBUG
	std::cout << "Creating swapchain" << std::endl;
#endif

	CheckVkResult(
		vkCreateSwapchainKHR(vulkanHandler->device, &swapchainCreateInfo, nullptr, &vulkanHandler->swapchain),
		"error creating swapchain"
	);
}

void GetSwapchainImages(FGGVulkanHandler *vulkanHandler) {
	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(vulkanHandler->device, vulkanHandler->swapchain, &swapchainImageCount, nullptr);
	vulkanHandler->swapchainImages.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(vulkanHandler->device, vulkanHandler->swapchain, &swapchainImageCount, &vulkanHandler->swapchainImages[0]);
}

void CreateSwapchainImageViews(FGGVulkanHandler* vulkanHandler) {
	vulkanHandler->swapchainImageViews.resize(vulkanHandler->swapchainImages.size());
	for (uint32_t i = 0; i < vulkanHandler->swapchainImageViews.size(); i++) {

		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = nullptr;
		imageViewCreateInfo.image = vulkanHandler->swapchainImages[i];

		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = vulkanHandler->swapchainImageFormat;

		imageViewCreateInfo.components.r = VK_COMPONENT_SWIZZLE_R;
		imageViewCreateInfo.components.g = VK_COMPONENT_SWIZZLE_G;
		imageViewCreateInfo.components.b = VK_COMPONENT_SWIZZLE_B;
		imageViewCreateInfo.components.a = VK_COMPONENT_SWIZZLE_A;

		imageViewCreateInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
		imageViewCreateInfo.subresourceRange.baseMipLevel = 0;
		imageViewCreateInfo.subresourceRange.levelCount = 1;
		imageViewCreateInfo.subresourceRange.baseArrayLayer = 0;
		imageViewCreateInfo.subresourceRange.layerCount = 1;

		CheckVkResult(
			vkCreateImageView(vulkanHandler->device, &imageViewCreateInfo, nullptr, &vulkanHandler->swapchainImageViews[i]),
			"error creating swapchain image view"
		);
	}
}

/*
*	Pipeline Stuff
*/

VkShaderModule CreateShaderModule(const VkDevice &device, const char* input, const char* output) {

	Compile_glslc_Shader(input, output);
	std::string shaderBinaryCode = Utilities::ReadCodeBinary(output);
	
	VkShaderModuleCreateInfo shaderModuleCreateInfo{};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.pNext = nullptr;
	shaderModuleCreateInfo.codeSize = shaderBinaryCode.size();
	shaderModuleCreateInfo.pCode = reinterpret_cast<const uint32_t*>(&shaderBinaryCode[0]);

#ifndef NDEBUG
	std::cout << "creating shader module using binary at: "
	<< output << std::endl;
#endif

	VkShaderModule shaderModule;
	CheckVkResult(
		vkCreateShaderModule(device, &shaderModuleCreateInfo, nullptr, &shaderModule),
		"error creating shader module"
	);

	return shaderModule;
}

VkPipelineViewportStateCreateInfo SetViewportState(const VkPhysicalDevice &physicalDevice, VkViewport *viewport, const VkSurfaceKHR &surface) {

#ifndef NDEBUG
	std::cout << "setting viewport state" << std::endl;
#endif

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	viewport->x = 0.0f;
	viewport->y = 0.0f;
	viewport->width  = static_cast<float>(surfaceCapabilities.currentExtent.width);
	viewport->height = static_cast<float>(surfaceCapabilities.currentExtent.height);
	viewport->minDepth = 0.0f;
	viewport->maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = surfaceCapabilities.currentExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.pNext = nullptr;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	return viewportStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo CreateRasterizer() {

#ifndef NDEBUG
	std::cout << "creating rasterizer" << std::endl;
#endif

	VkPipelineRasterizationStateCreateInfo rasterizationStateCreateInfo{};
	rasterizationStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
	rasterizationStateCreateInfo.pNext = nullptr;
	rasterizationStateCreateInfo.depthClampEnable = VK_FALSE;
	rasterizationStateCreateInfo.polygonMode = VK_POLYGON_MODE_FILL;
	rasterizationStateCreateInfo.lineWidth = 1.0f;
	rasterizationStateCreateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
	rasterizationStateCreateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
	rasterizationStateCreateInfo.depthBiasEnable = VK_FALSE;
	rasterizationStateCreateInfo.depthBiasConstantFactor = 0.0f;
	rasterizationStateCreateInfo.depthBiasClamp = 0.0f; 
	rasterizationStateCreateInfo.depthBiasSlopeFactor = 0.0f;

	return rasterizationStateCreateInfo;
}

VkPipelineMultisampleStateCreateInfo EnableMSAA() {

#ifndef NDEBUG
	std::cout << "MSAA settings..." << std::endl;
#endif

	VkPipelineMultisampleStateCreateInfo MSAACreateInfo{};
	MSAACreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
	MSAACreateInfo.pNext = nullptr;
	MSAACreateInfo.sampleShadingEnable = VK_FALSE;
	MSAACreateInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
	
	return MSAACreateInfo;
}

VkPipelineColorBlendStateCreateInfo ColorBlendSettings() {

#ifndef NDEBUG
	std::cout << "setting up color blend settings" << std::endl;
#endif

	VkPipelineColorBlendAttachmentState colorBlendAttachmentState{};
	colorBlendAttachmentState.colorWriteMask =
		VK_COLOR_COMPONENT_R_BIT |
		VK_COLOR_COMPONENT_G_BIT |
		VK_COLOR_COMPONENT_B_BIT |
		VK_COLOR_COMPONENT_A_BIT;
	colorBlendAttachmentState.blendEnable = VK_FALSE;

	VkPipelineColorBlendStateCreateInfo colorBlendStateCreateInfo{};
	colorBlendStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
	colorBlendStateCreateInfo.pNext = nullptr;
	colorBlendStateCreateInfo.attachmentCount = 1;
	colorBlendStateCreateInfo.pAttachments = &colorBlendAttachmentState;
	colorBlendStateCreateInfo.logicOpEnable = VK_FALSE; //bitwise blending

	return colorBlendStateCreateInfo;
}

VkPipelineDynamicStateCreateInfo SetDynamicState(const VkDynamicState *dynamicStates, const uint32_t &dynamicStateCount) {

#ifndef NDEBUG
	std::cout << "setting pipeline dynamic state" << std::endl;
#endif

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.pNext = nullptr;
	dynamicStateCreateInfo.dynamicStateCount = dynamicStateCount;
	dynamicStateCreateInfo.pDynamicStates = dynamicStates;

	return dynamicStateCreateInfo;
}

VkPipelineLayout SetPipelineLayout(const VkDevice &device) {

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	
#ifndef NDEBUG
	std::cout << "setting up pipeline layout" << std::endl;
#endif

	VkPipelineLayout pipelineLayout;
	CheckVkResult(
		vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &pipelineLayout),
		"error creating pipeline layout"
	);

	return pipelineLayout;
}

void CreateRenderPass(const VkFormat &swapchainImageFormat, const VkDevice device, VkRenderPass *renderPass) {

	VkAttachmentDescription colorAttachmentDescription{};
	colorAttachmentDescription.format = swapchainImageFormat;
	colorAttachmentDescription.samples = VK_SAMPLE_COUNT_1_BIT;

	colorAttachmentDescription.loadOp  = VK_ATTACHMENT_LOAD_OP_CLEAR; //clear framebuffer data when not drawing
	colorAttachmentDescription.storeOp = VK_ATTACHMENT_STORE_OP_STORE; //store framebuffer data when created

	colorAttachmentDescription.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE; //not using stencil data
	colorAttachmentDescription.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE; //not using stencil data
	
	//image layout
	//initial/final layout: which layout the image will have before/after the render pass
	colorAttachmentDescription.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //doesn't matter
	colorAttachmentDescription.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR; //present image to the swapchain

	//a render pass is made of subpasses, stored in VkAttachmentReference
	VkAttachmentReference colorAttachmentReference{};
	colorAttachmentReference.attachment = 0; //from the fragment shader layout(location = 0)
	colorAttachmentReference.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL; //seems to be default

	//The subpass is described using a VkSubpassDescription structure:
	VkSubpassDescription subpassDescription{};
	subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS; //this is a graphics subpass
	subpassDescription.colorAttachmentCount = 1;
	subpassDescription.pColorAttachments = &colorAttachmentReference;


	VkSubpassDependency subpassDependency{};
	subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
	subpassDependency.dstSubpass = 0; //index of the subpass
	subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.srcAccessMask = 0;
	subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
	subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

	VkRenderPassCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;
	renderPassCreateInfo.dependencyCount = 1;
	renderPassCreateInfo.pDependencies = &subpassDependency;

#ifndef NDEBUG
	std::cout << "creating render pass" << std::endl;
#endif

	CheckVkResult(
		vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, renderPass),
		"error creating render pass"
	);
}

void CreateGraphicsPipeline(FGGVulkanHandler *vulkanHandler, VkPipeline *graphicsPipeline) {

	VkShaderModule vertexShaderModule = CreateShaderModule(vulkanHandler->device, "../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	VkShaderModule fragmentShaderModule = CreateShaderModule(vulkanHandler->device, "../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");

	std::array<VkShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };

	VkPipelineShaderStageCreateInfo vertexShaderStageCreateInfo{};
	vertexShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	vertexShaderStageCreateInfo.pNext = nullptr;
	vertexShaderStageCreateInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
	vertexShaderStageCreateInfo.module = vertexShaderModule;
	vertexShaderStageCreateInfo.pName = "main";
	vertexShaderStageCreateInfo.pSpecializationInfo = nullptr;

	VkPipelineShaderStageCreateInfo fragmentShaderStageCreateInfo{};
	fragmentShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	fragmentShaderStageCreateInfo.pNext = nullptr;
	fragmentShaderStageCreateInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
	fragmentShaderStageCreateInfo.module = fragmentShaderModule;
	fragmentShaderStageCreateInfo.pName = "main";
	fragmentShaderStageCreateInfo.pSpecializationInfo = nullptr;

	std::array<VkPipelineShaderStageCreateInfo, 2> shaderStages = {
		vertexShaderStageCreateInfo, fragmentShaderStageCreateInfo
	};

	VkPipelineVertexInputStateCreateInfo vertexInputCreateInfo{};
	vertexInputCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
	vertexInputCreateInfo.pNext = nullptr;
	vertexInputCreateInfo.vertexBindingDescriptionCount = 0;
	vertexInputCreateInfo.pVertexBindingDescriptions = nullptr;
	vertexInputCreateInfo.pVertexAttributeDescriptions = nullptr;

	VkPipelineInputAssemblyStateCreateInfo inputAssemblyStateCreateInfo{};
	inputAssemblyStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
	inputAssemblyStateCreateInfo.pNext = nullptr;
	inputAssemblyStateCreateInfo.flags = 0;
	inputAssemblyStateCreateInfo.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = nullptr;
	graphicsPipelineCreateInfo.stageCount = (uint32_t)shaderStages.size();
	graphicsPipelineCreateInfo.pStages = &shaderStages[0];
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;

	VkPipelineViewportStateCreateInfo viewportState = SetViewportState(vulkanHandler->physicalDevice, &vulkanHandler->viewport, vulkanHandler->surface);
	graphicsPipelineCreateInfo.pViewportState = &viewportState;

	VkPipelineColorBlendStateCreateInfo blendState = ColorBlendSettings();
	graphicsPipelineCreateInfo.pColorBlendState = &blendState;

	VkPipelineDynamicStateCreateInfo dynamicState = SetDynamicState(&vulkanHandler->dynamicStates[0], (uint32_t)vulkanHandler->dynamicStates.size());
	graphicsPipelineCreateInfo.pDynamicState = &dynamicState;
	
	VkPipelineMultisampleStateCreateInfo multiSampleState = EnableMSAA();
	graphicsPipelineCreateInfo.pMultisampleState = &multiSampleState;
	
	VkPipelineRasterizationStateCreateInfo rasterizationState = CreateRasterizer();
	graphicsPipelineCreateInfo.pRasterizationState = &rasterizationState;
	
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	
	graphicsPipelineCreateInfo.layout = SetPipelineLayout(vulkanHandler->device);
	graphicsPipelineCreateInfo.renderPass = vulkanHandler->renderPass;
	graphicsPipelineCreateInfo.subpass = 0;

#ifndef NDEBUG
	std::cout << "creating graphics pipeline" << std::endl;
#endif

	CheckVkResult(
		vkCreateGraphicsPipelines(vulkanHandler->device, nullptr, 1, &graphicsPipelineCreateInfo, nullptr, graphicsPipeline),
		"error creating graphics pipeline"
	);
}

/*
*	Framebuffers
*/

void CreateFramebuffers(FGGVulkanHandler *vulkanHandler) {
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanHandler->physicalDevice, vulkanHandler->surface, &surfaceCapabilities);

	for (size_t i = 0; i < vulkanHandler->swapchainImageViews.size(); i++) {
		
		VkImageView attachments[] = {
			vulkanHandler->swapchainImageViews[i]
		};

		VkFramebufferCreateInfo framebufferCreateInfo{};
		framebufferCreateInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
		framebufferCreateInfo.pNext = nullptr;
		framebufferCreateInfo.attachmentCount = 1;
		framebufferCreateInfo.pAttachments = attachments;
		framebufferCreateInfo.width = surfaceCapabilities.currentExtent.width;
		framebufferCreateInfo.height = surfaceCapabilities.currentExtent.height;
		framebufferCreateInfo.layers = 1;
		framebufferCreateInfo.renderPass = vulkanHandler->renderPass;

		VkFramebuffer framebuffer;
		CheckVkResult(
			vkCreateFramebuffer(vulkanHandler->device, &framebufferCreateInfo, nullptr, &framebuffer),
			(std::string("error creating framebuffer at index ") + std::to_string(i)).c_str()
		);

		vulkanHandler->swapchainFramebuffers.push_back(framebuffer);
	}
}

/*
*	Drawing
*/

void CmdBufferRecordStart(const FGGVulkanHandler& vulkanHandler) {

	for (const VkCommandBuffer cmdBuffer : vulkanHandler.cmdBuffers) {
		VkCommandBufferBeginInfo cmdBufferBeginInfo{};
		cmdBufferBeginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
		cmdBufferBeginInfo.pNext = nullptr;
		cmdBufferBeginInfo.flags = VK_COMMAND_BUFFER_USAGE_SIMULTANEOUS_USE_BIT;
		cmdBufferBeginInfo.pInheritanceInfo = nullptr;

		CheckVkResult(
			vkBeginCommandBuffer(cmdBuffer, &cmdBufferBeginInfo),
			"error recording command buffer (begin)"
		);
	}
}

void RenderPassStart(const FGGVulkanHandler &vulkanHandler) {
	
	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vulkanHandler.physicalDevice, vulkanHandler.surface, &surfaceCapabilities);

	for (size_t i = 0; i < vulkanHandler.cmdBuffers.size(); i++) {
		
		VkRenderPassBeginInfo renderPassBeginInfo{};
		renderPassBeginInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
		renderPassBeginInfo.pNext = nullptr;
		renderPassBeginInfo.renderPass = vulkanHandler.renderPass;
		renderPassBeginInfo.framebuffer = vulkanHandler.swapchainFramebuffers[i];
		renderPassBeginInfo.renderArea.offset = { 0, 0 };
		renderPassBeginInfo.renderArea.extent = surfaceCapabilities.currentExtent;
		VkClearValue clearColor = { 0.0f, 0.0f, 0.0f, 1.0f };
		renderPassBeginInfo.clearValueCount = 1;
		renderPassBeginInfo.pClearValues = &clearColor;

		vkCmdBeginRenderPass(vulkanHandler.cmdBuffers[i], &renderPassBeginInfo, VK_SUBPASS_CONTENTS_INLINE); //use primary command buffers
	}
	
}

void RenderPassEnd(VkRenderPass renderPass, const VkCommandBuffer *cmdBuffers, size_t cmdBufferCount) {

	for (size_t i = 0; i < cmdBufferCount; i++) {
		vkCmdEndRenderPass(cmdBuffers[i]);
	}
}

void CmdBufferRecordStop(const VkCommandBuffer* cmdBuffers, size_t cmdBufferCount) {
	for (size_t i = 0; i < cmdBufferCount; i++) {
		CheckVkResult(
			vkEndCommandBuffer(cmdBuffers[i]),
			"error recording command buffer (end)"
		);
	}
}

void GraphicsPipelineDraw(const VkCommandBuffer *cmdBuffers, const size_t cmdBufferCount, const VkViewport &viewport, const VkPipeline& graphicsPipeline) {
	for (size_t i = 0; i < cmdBufferCount; i++) {
		
		vkCmdSetViewport(cmdBuffers[i], 0, 1, &viewport);
		vkCmdBindPipeline(cmdBuffers[i], VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);
		vkCmdDraw(cmdBuffers[i], 3, 1, 0, 0);
	}
}

/*
*	Semaphores
*/

void CreateSemaphore(const VkDevice& device, VkSemaphore *semaphore) {
	VkSemaphoreCreateInfo semaphoreCreateInfo{};
	semaphoreCreateInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
	semaphoreCreateInfo.pNext = nullptr;
	semaphoreCreateInfo.flags = 0;
	
	CheckVkResult(
		vkCreateSemaphore(device, &semaphoreCreateInfo, nullptr, semaphore),
		"error creating semaphore"
	);
}

void PresentFrame(const FGGVulkanHandler &vulkanHandler) {

	uint32_t imageIndex;
	vkAcquireNextImageKHR(vulkanHandler.device, vulkanHandler.swapchain, UINT64_MAX, vulkanHandler.imageAvailableSemaphore, nullptr, &imageIndex);

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	
	
	// which semaphore to wait on before execution begins and which stages of the pipeline to wait (writing colors)
	VkSemaphore waitSemaphores[] = {vulkanHandler.imageAvailableSemaphore};
	VkPipelineStageFlags waitStages[] = {VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT};
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;

	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &vulkanHandler.cmdBuffers[imageIndex];

	// which semaphore to signal once the cmd buffers have finished execution
	VkSemaphore signalSemaphores[] = { vulkanHandler.renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	CheckVkResult(
		vkQueueSubmit(vulkanHandler.queues[vulkanHandler.graphicsQueueIndex], 1, &submitInfo, nullptr),
		"error submitting draw command buffer"
	);

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = &signalSemaphores[0];
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = &vulkanHandler.swapchain;
	presentInfo.pImageIndices = &imageIndex;

	CheckVkResult(
		vkQueuePresentKHR(vulkanHandler.queues[vulkanHandler.graphicsQueueIndex], &presentInfo),
		"error presenting queue"
	);
}

/*
*	Cleanup
*/

void Cleanup(FGGVulkanHandler* vulkanHandler) {
	vkDestroySemaphore(vulkanHandler->device, vulkanHandler->imageAvailableSemaphore, nullptr);
	vkDestroySemaphore(vulkanHandler->device, vulkanHandler->renderFinishedSemaphore, nullptr);
	for (const VkFramebuffer &framebuffer : vulkanHandler->swapchainFramebuffers) {
		vkDestroyFramebuffer(vulkanHandler->device, framebuffer, nullptr);
	}
	vkDestroyPipeline(vulkanHandler->device, vulkanHandler->graphicsPipeline, nullptr);
	vkDestroyPipelineLayout(vulkanHandler->device, vulkanHandler->pipelineLayout, nullptr);
	vkDestroyRenderPass(vulkanHandler->device, vulkanHandler->renderPass, nullptr);
	vkDestroyPipelineLayout(vulkanHandler->device, vulkanHandler->pipelineLayout, nullptr);
	for (const VkShaderModule& shaderModule : vulkanHandler->shaderModules) {
		vkDestroyShaderModule(vulkanHandler->device, shaderModule, nullptr);
	}
	for (const VkImageView &imageview : vulkanHandler->swapchainImageViews) {
		vkDestroyImageView(vulkanHandler->device, imageview, nullptr);
	}
	for (const VkCommandPool& cmdPool : vulkanHandler->cmdPools) {
		vkDestroyCommandPool(vulkanHandler->device, cmdPool, nullptr);
	}
	vkDestroySwapchainKHR(vulkanHandler->device, vulkanHandler->swapchain, nullptr);
	vkDestroyDevice(vulkanHandler->device, nullptr);
	ClearWindow(&vulkanHandler->window);
	vkDestroyInstance(vulkanHandler->instance, nullptr);
}

#ifndef NDEBUG

bool CheckValidationLayers(const FGGVulkanHandler& vulkanHandler) {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers[0]);

	size_t requestedLayers = vulkanHandler.requiredValidationLayers.size();

	std::cout << "Installed validation layers: " << std::endl;
	for (uint32_t i = 0; i < (uint32_t)vulkanHandler.requiredValidationLayers.size(); i++) {
		for (const VkLayerProperties& properties : availableLayers) {
			std::cout << properties.layerName << " - VERSION " << properties.implementationVersion <<
				": " << properties.description << std::endl;
			if (strcmp(properties.layerName, vulkanHandler.requiredValidationLayers[i]) == 0) {
				requestedLayers -= 1;
			}
		}
	}
	std::cout << "Required validation layers: " << std::endl;
	for (const char* requiredLayer : vulkanHandler.requiredValidationLayers) {
		std::cout << requiredLayer << std::endl;
	}

	if (requestedLayers > 0) {
		return 0;
	}

	return 1;
}

#endif

/*
*	Utilities
*/

void CheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		std::cout << errormsg << ": " << TranslateVkResult(result) << std::endl;
#endif	
		throw std::runtime_error(errormsg);
	}
}

const char* TranslateVkResult(const VkResult& vkResult) {
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

const char* TranslateQueueFlags(const VkQueueFlags& queueFlag) {
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
	std::string cmd = std::string("glslc ") + input + " -o " + output;
#ifndef NDEBUG	
	std::cout << cmd << std::endl;
#endif
	system(cmd.c_str());
}