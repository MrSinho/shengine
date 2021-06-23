#include <stdexcept>

#ifndef NDEBUG
	#include <iostream>
#endif

#include "VulkanHandler.h"
#include "Utilities.h"

#pragma warning ( disable : 26812 )

#ifndef NDEBUG

void VulkanHandler::EnableValidationLayers() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers[0]);

	size_t requestedLayers = requiredValidationLayers.size();

	std::cout << "Installed validation layers: " << std::endl;
	for (uint32_t i = 0; i < static_cast<uint32_t>(requiredValidationLayers.size()); i++) {
		for (const VkLayerProperties& properties : availableLayers) {
			std::cout << properties.layerName << " - VERSION " << properties.implementationVersion <<
				": " << properties.description << std::endl;
			if (strcmp(properties.layerName, requiredValidationLayers[i]) == 0) {
				requestedLayers -= 1;
			}
		}
	}
	std::cout << "Required validation layers: " << std::endl;
	for (const char* requiredLayer : requiredValidationLayers) {
		std::cout << requiredLayer << std::endl;
	}

	if (requestedLayers > 0) {
		throw std::runtime_error("Not all the required validation layers have been found");
	}
}

#endif

void VulkanHandler::CheckVkResult(VkResult result, const char* errormsg) {
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		std::cout << errormsg << ": " << TranslateVkResult(result) << std::endl;
#endif	
		throw std::runtime_error(errormsg);
	}
}

void VulkanHandler::InitVulkan(uint32_t width, uint32_t height, const char* title) {
	window = Window();
	window.InitGLFW(width, height, title);
#ifndef NDEBUG
	EnableValidationLayers();
#endif
	CreateInstance();
	CreateWindowSurface();
	SetPhysicalDevice();
	SetLogicalDevice();
	CreateSwapchain();
	GetSwapchainImages();
	CreateSwapchainImageViews();
	CreateRenderPass();
	CreateGraphicsPipeline();
}

void VulkanHandler::CreateInstance() {
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
	createInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
	createInfo.ppEnabledLayerNames = &requiredValidationLayers[0];
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
		vkCreateInstance(&createInfo, nullptr, &instance), 
		"error creating vkinstance"
	);
}

void VulkanHandler::CreateWindowSurface() {
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo{};
	win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	win32SurfaceCreateInfo.pNext = nullptr;
	win32SurfaceCreateInfo.hwnd = glfwGetWin32Window(window.window);
	win32SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
#endif

	CheckVkResult(
		vkCreateWin32SurfaceKHR(instance, &win32SurfaceCreateInfo, nullptr, &surface), 
		"error creating window surface"
	);
}

const char* VulkanHandler::TranslateVkResult(const VkResult& vkResult) {
	switch (vkResult) {
		case VK_SUCCESS							:return "VK_SUCCESS";
		case VK_NOT_READY						:return "VK_NOT_READY";
		case VK_TIMEOUT 						:return "VK_TIMEOUT";
		case VK_EVENT_SET						:return "VK_EVENT_SET";
		case VK_EVENT_RESET 					:return "VK_EVENT_RESET";
		case VK_INCOMPLETE						:return "VK_INCOMPLETE";
		case VK_ERROR_OUT_OF_HOST_MEMORY		:return "VK_ERROR_OUT_OF_HOST_MEMORY";
		case VK_ERROR_OUT_OF_DEVICE_MEMORY 		:return "VK_ERROR_OUT_OF_DEVICE_MEMORY";
		case VK_ERROR_INITIALIZATION_FAILED 	:return "VK_ERROR_INITIALIZATION_FAILED";
		case VK_ERROR_DEVICE_LOST 				:return "VK_ERROR_DEVICE_LOST";
		case VK_ERROR_MEMORY_MAP_FAILED 		:return "VK_ERROR_MEMORY_MAP_FAILED";
		case VK_ERROR_LAYER_NOT_PRESENT 		:return "VK_ERROR_LAYER_NOT_PRESENT";
		case VK_ERROR_EXTENSION_NOT_PRESENT 	:return "VK_ERROR_EXTENSION_NOT_PRESENT";
		case VK_ERROR_FEATURE_NOT_PRESENT 		:return "VK_ERROR_FEATURE_NOT_PRESENT";
		case VK_ERROR_INCOMPATIBLE_DRIVER		:return "VK_ERROR_INCOMPATIBLE_DRIVER";
		case VK_ERROR_TOO_MANY_OBJECTS			:return "VK_ERROR_TOO_MANY_OBJECTS";
		case VK_ERROR_FORMAT_NOT_SUPPORTED 		:return "VK_ERROR_FORMAT_NOT_SUPPORTED";
		case VK_ERROR_SURFACE_LOST_KHR			:return "VK_ERROR_SURFACE_LOST_KHR";
		case VK_SUBOPTIMAL_KHR 					:return "VK_SUBOPTIMAL_KHR";
		case VK_ERROR_OUT_OF_DATE_KHR			:return "VK_ERROR_OUT_OF_DATE_KHR";
		case VK_ERROR_INCOMPATIBLE_DISPLAY_KHR 	:return "VK_ERROR_INCOMPATIBLE_DISPLAY_KHR";
		case VK_ERROR_NATIVE_WINDOW_IN_USE_KHR 	:return "VK_ERROR_NATIVE_WINDOW_IN_USE_KHR";
		case VK_ERROR_VALIDATION_FAILED_EXT		:return "VK_ERROR_VALIDATION_FAILED_EXT";
		case VK_RESULT_MAX_ENUM					:return "VK_RESULT_MAX_ENUM";
	}
	return "unknown vkresult";
}

void VulkanHandler::SetPhysicalDevice() {

	uint32_t pDeviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &pDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> pDevices(pDeviceCount);
	vkEnumeratePhysicalDevices(instance, &pDeviceCount, &pDevices[0]);

	if (pDeviceCount == 0) {
		throw std::runtime_error("No Vulkan compatible gpu has been found");
	}

	// GET IF DEVICE IS SUITABLE
	std::vector<VkPhysicalDevice> suitableDevices;
	std::vector<std::set<uint32_t>> suitableDevicesQueueFamilyIndices;

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

		for (const VkQueueFlags &queueFlag : requiredQueueFlags) {
			for (uint32_t i = 0; i < queueFamilyPropertyCount; i++) {
				if (pQueueFamilyProperties[i].queueFlags & queueFlag) {
					_queueFamilyIndices.insert(i);
				}
				VkBool32 __surfaceSupport;
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, i, surface, &__surfaceSupport);
				if (static_cast<bool>(__surfaceSupport) && !_surfaceSupport) {
					_surfaceSupport = 1;
					_queueFamilyIndices.insert(i);
				}
			}
		}
		if (!_queueFamilyIndices.empty() && _surfaceSupport) { 
			suitableDevices.push_back(pDevice);
			suitableDevicesQueueFamilyIndices.push_back(_queueFamilyIndices);
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
				physicalDevice = suitableDevices[i];
				queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[i].begin(), suitableDevicesQueueFamilyIndices[i].end());
			}
			else {
				physicalDevice = suitableDevices[i - 1];
				queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[i - 1].begin(), suitableDevicesQueueFamilyIndices[i - 1].end());
			}
		}
	}
	else {
		physicalDevice = suitableDevices[0];
		queueFamilyIndices.assign(suitableDevicesQueueFamilyIndices[0].begin(), suitableDevicesQueueFamilyIndices[0].end());
	}

#ifndef NDEBUG
	VkPhysicalDeviceProperties pDeviceProperties;
	vkGetPhysicalDeviceProperties(physicalDevice, &pDeviceProperties);
	std::cout << "Using " << pDeviceProperties.deviceName << std::endl;
#endif
}

bool VulkanHandler::CheckPhysicalDeviceExtensions(const VkPhysicalDevice& pDevice) {
	uint32_t extensionCount = 0;
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, nullptr);
	
	std::vector<VkExtensionProperties> pDeviceExtensions(extensionCount);
	vkEnumerateDeviceExtensionProperties(pDevice, nullptr, &extensionCount, &pDeviceExtensions[0]);

#ifndef NDEBUG
	std::cout << "Enumerating device extensions" << std::endl;
#endif
	uint32_t requiredExtensionsCount = static_cast<uint32_t>(requiredDeviceExtensionsNames.size());
	for (const VkExtensionProperties& extensionProperties : pDeviceExtensions) {
		for (const char* requiredName : requiredDeviceExtensionsNames) {
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

const char* VulkanHandler::TranslateQueueFlags(const VkQueueFlags& queueFlag) {
		if(queueFlag & VK_QUEUE_GRAPHICS_BIT)			{ return "VK_QUEUE_GRAPHICS_BIT";			}
		if(queueFlag & VK_QUEUE_COMPUTE_BIT)			{ return "VK_QUEUE_COMPUTE_BIT";			}
		if(queueFlag & VK_QUEUE_TRANSFER_BIT)			{ return "VK_QUEUE_TRANSFER_BIT";			}
		if(queueFlag & VK_QUEUE_SPARSE_BINDING_BIT)		{ return "VK_QUEUE_SPARSE_BINDING_BIT";		}
		if(queueFlag & VK_QUEUE_PROTECTED_BIT)			{ return "VK_QUEUE_PROTECTED_BIT";			}
#ifdef VK_ENABLE_BETA_EXTENSIONS
		if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) { return "VK_QUEUE_VIDEO_DECODE_BIT_KHR";	}
#endif											
#ifdef VK_ENABLE_BETA_EXTENSIONS				
		if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) { return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR";	}
#endif											
		if (queueFlag & VK_QUEUE_FLAG_BITS_MAX_ENUM)	{ return "VK_QUEUE_FLAG_BITS_MAX_ENUM";		}
	return "unknown flag";
}

VkDeviceQueueCreateInfo VulkanHandler::CreateQueue(uint32_t queueFamilyIndex) {
	float queuePriority = 1.0f;
	VkDeviceQueueCreateInfo queueCreateInfo{};
	queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
	queueCreateInfo.pQueuePriorities = &queuePriority;
	queueCreateInfo.queueCount = 1;
	queueCreateInfo.queueFamilyIndex = queueFamilyIndex;

	return queueCreateInfo;
}

void VulkanHandler::SetLogicalDevice() {
	
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	for (const uint32_t& index : queueFamilyIndices) {
		queueCreateInfos.push_back(CreateQueue(index));
	}

	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
#ifndef NDEBUG
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
	deviceCreateInfo.ppEnabledLayerNames = &requiredValidationLayers[0];
#endif
	deviceCreateInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
	deviceCreateInfo.pQueueCreateInfos = &queueCreateInfos[0];
	VkPhysicalDeviceFeatures pDeviceFeatures{};
	deviceCreateInfo.pEnabledFeatures = &pDeviceFeatures;
	deviceCreateInfo.enabledExtensionCount = static_cast<uint32_t>(requiredDeviceExtensionsNames.size());
	deviceCreateInfo.ppEnabledExtensionNames = &requiredDeviceExtensionsNames[0];

#ifndef NDEBUG
	std::cout << "Creating logical device" << std::endl;
#endif

	CheckVkResult(
		vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device), 
		"error creating logical device"
	);

	for (const uint32_t& index : queueFamilyIndices) {
		VkCommandPool cmdPool = CreateCommandPool(index);
		CreateCmdBuffer(cmdPool);
		cmdPools.push_back(cmdPool);
	}
}

VkCommandPool VulkanHandler::CreateCommandPool(uint32_t queueFamilyIndex) {

	VkCommandPoolCreateInfo cmdPoolCreateInfo;
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.pNext = nullptr;
	cmdPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;

#ifndef NDEBUG
	uint32_t queueFamilyPropertiesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, &queueFamilyProperties[0]);
	std::cout << "Creating " << TranslateQueueFlags(queueFamilyProperties[queueFamilyIndex].queueFlags) << " command pool" << std::endl;
#endif

	VkCommandPool cmdPool = nullptr;
	CheckVkResult(
		vkCreateCommandPool(device, &cmdPoolCreateInfo, nullptr, &cmdPool), 
		"error creating command pool"
	);

	return cmdPool;
}

void VulkanHandler::CreateCmdBuffer(const VkCommandPool &cmdPool) {
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
}

void VulkanHandler::CreateSwapchain() {

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	uint32_t surfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, &surfaceFormats[0]);
	
	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, &presentModes[0]);

	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = nullptr;
	swapchainCreateInfo.surface = surface;
	
	swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
	
	if (surfaceFormatsCount == 1 && surfaceFormats[0].format == VK_FORMAT_UNDEFINED) {
		swapchainImageFormat = VK_FORMAT_B8G8R8A8_UNORM;
	}
	else if (surfaceFormatsCount > 1) {
		swapchainImageFormat = surfaceFormats[0].format;
	}
	else { throw std::runtime_error("no supported image format available"); }
	swapchainCreateInfo.imageFormat = swapchainImageFormat;

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
	if (queueFamilyIndices.size() == 1) {
		swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}
	swapchainCreateInfo.queueFamilyIndexCount = static_cast<uint32_t>(queueFamilyIndices.size());
	swapchainCreateInfo.pQueueFamilyIndices = &queueFamilyIndices[0];

#ifndef NDEBUG
	std::cout << "Creating swapchain" << std::endl;
#endif

	CheckVkResult(
		vkCreateSwapchainKHR(device, &swapchainCreateInfo, nullptr, &swapchain),
		"error creating swapchain"
	);
}

void VulkanHandler::GetSwapchainImages() {
	uint32_t swapchainImageCount;
	vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, nullptr);
	swapchainImages.resize(swapchainImageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &swapchainImageCount, &swapchainImages[0]);
}

void VulkanHandler::CreateSwapchainImageViews() {
	swapchainImageViews.resize(swapchainImages.size());
	for (uint32_t i = 0; i < swapchainImageViews.size(); i++) {
		
		VkImageViewCreateInfo imageViewCreateInfo{};
		imageViewCreateInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
		imageViewCreateInfo.pNext = nullptr;
		imageViewCreateInfo.image = swapchainImages[i];

		imageViewCreateInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
		imageViewCreateInfo.format = swapchainImageFormat;

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
			vkCreateImageView(device, &imageViewCreateInfo, nullptr, &swapchainImageViews[i]),
			"error creating swapchain image view"
		);
	}
}

void VulkanHandler::CompileShader(const char* input, const char *output) {
	std::string cmd = std::string("glslc ") + input + " -o " + output;
#ifndef NDEBUG	
	std::cout << cmd << std::endl;
#endif
	system(cmd.c_str());
}

VkShaderModule VulkanHandler::CreateShaderModule(const char* input, const char* output) {

	CompileShader(input, output);
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

VkPipelineViewportStateCreateInfo VulkanHandler::SetViewportState() {

#ifndef NDEBUG
	std::cout << "setting viewport state" << std::endl;
#endif

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width  = static_cast<float>(surfaceCapabilities.currentExtent.width);
	viewport.height = static_cast<float>(surfaceCapabilities.currentExtent.height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;

	VkRect2D scissor{};
	scissor.offset = {0, 0};
	scissor.extent = surfaceCapabilities.currentExtent;

	VkPipelineViewportStateCreateInfo viewportStateCreateInfo{};
	viewportStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
	viewportStateCreateInfo.pNext = nullptr;
	viewportStateCreateInfo.viewportCount = 1;
	viewportStateCreateInfo.pViewports = &viewport;
	viewportStateCreateInfo.scissorCount = 1;
	viewportStateCreateInfo.pScissors = &scissor;

	return viewportStateCreateInfo;
}

VkPipelineRasterizationStateCreateInfo VulkanHandler::CreateRasterizer() {

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

VkPipelineMultisampleStateCreateInfo VulkanHandler::EnableMSAA() {

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

VkPipelineColorBlendStateCreateInfo VulkanHandler::ColorBlendSettings() {

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

VkPipelineDynamicStateCreateInfo VulkanHandler::SetDynamicState() {

#ifndef NDEBUG
	std::cout << "setting pipeline dynamic state" << std::endl;
#endif

	VkPipelineDynamicStateCreateInfo dynamicStateCreateInfo{};
	dynamicStateCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
	dynamicStateCreateInfo.pNext = nullptr;
	dynamicStateCreateInfo.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
	dynamicStateCreateInfo.pDynamicStates = &dynamicStates[0];

	return dynamicStateCreateInfo;
}

VkPipelineLayout VulkanHandler::SetPipelineLayout() {

	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.pNext = nullptr;
	
#ifndef NDEBUG
	std::cout << "setting up pipeline layout" << std::endl;
#endif

	VkPipelineLayout _pipelineLayout = nullptr;
	CheckVkResult(
		vkCreatePipelineLayout(device, &pipelineLayoutCreateInfo, nullptr, &_pipelineLayout),
		"error creating pipeline layout"
	);

	pipelineLayout = _pipelineLayout;
	return _pipelineLayout;
}

VkRenderPass VulkanHandler::CreateRenderPass() {

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

	VkRenderPassCreateInfo renderPassCreateInfo{};
	renderPassCreateInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
	renderPassCreateInfo.attachmentCount = 1;
	renderPassCreateInfo.pAttachments = &colorAttachmentDescription;
	renderPassCreateInfo.subpassCount = 1;
	renderPassCreateInfo.pSubpasses = &subpassDescription;

#ifndef NDEBUG
	std::cout << "creating render pass" << std::endl;
#endif

	CheckVkResult(
		vkCreateRenderPass(device, &renderPassCreateInfo, nullptr, &renderPass),
		"error creating render pass"
	);

	return renderPass;
}

void VulkanHandler::CreateGraphicsPipeline() {

	VkShaderModule vertexShaderModule = CreateShaderModule("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	VkShaderModule fragmentShaderModule = CreateShaderModule("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");

	shaderModules.push_back(vertexShaderModule);
	shaderModules.push_back(fragmentShaderModule);

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
	inputAssemblyStateCreateInfo.flags = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
	inputAssemblyStateCreateInfo.primitiveRestartEnable = VK_FALSE;


	VkGraphicsPipelineCreateInfo graphicsPipelineCreateInfo{};
	graphicsPipelineCreateInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
	graphicsPipelineCreateInfo.pNext = nullptr;
	graphicsPipelineCreateInfo.stageCount = static_cast<uint32_t>(shaderStages.size());
	graphicsPipelineCreateInfo.pStages = &shaderStages[0];
	graphicsPipelineCreateInfo.pVertexInputState = &vertexInputCreateInfo;
	graphicsPipelineCreateInfo.pInputAssemblyState = &inputAssemblyStateCreateInfo;
	graphicsPipelineCreateInfo.pViewportState = &SetViewportState();
	graphicsPipelineCreateInfo.pColorBlendState = &ColorBlendSettings();
	graphicsPipelineCreateInfo.pDynamicState = &SetDynamicState();
	graphicsPipelineCreateInfo.pMultisampleState = &EnableMSAA();
	graphicsPipelineCreateInfo.pRasterizationState = &CreateRasterizer();
	graphicsPipelineCreateInfo.pDepthStencilState = nullptr;
	
	graphicsPipelineCreateInfo.layout = SetPipelineLayout();
	graphicsPipelineCreateInfo.renderPass = CreateRenderPass();
	graphicsPipelineCreateInfo.subpass = 0;
	graphicsPipelineCreateInfo.basePipelineHandle = nullptr;
	graphicsPipelineCreateInfo.basePipelineIndex = -1;
}

void VulkanHandler::Cleanup() {
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	vkDestroyRenderPass(device, renderPass, nullptr);
	vkDestroyPipelineLayout(device, pipelineLayout, nullptr);
	for (const VkShaderModule& shaderModule : shaderModules) {
		vkDestroyShaderModule(device, shaderModule, nullptr);
	}
	for (const VkImageView &imageview : swapchainImageViews) {
		vkDestroyImageView(device, imageview, nullptr);
	}
	for (const VkCommandPool& cmdPool : cmdPools) {
		vkDestroyCommandPool(device, cmdPool, nullptr);
	}
	vkDestroySwapchainKHR(device, swapchain, nullptr);
	vkDestroyDevice(device, nullptr);
	window.Clear();
	vkDestroyInstance(instance, nullptr);
}