#include <stdexcept>
#include <string>

#ifndef NDEBUG
	#include <iostream>
#endif

#include "VulkanHandler.h"

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

	CheckVkResult(vkCreateInstance(&createInfo, nullptr, &instance), "Error creating vkinstance");
}

void VulkanHandler::CreateWindowSurface() {
#ifdef WIN32
	VkWin32SurfaceCreateInfoKHR win32SurfaceCreateInfo{};
	win32SurfaceCreateInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
	win32SurfaceCreateInfo.pNext = VK_NULL_HANDLE;
	win32SurfaceCreateInfo.hwnd = glfwGetWin32Window(window.window);
	win32SurfaceCreateInfo.hinstance = GetModuleHandle(nullptr);
#endif

	CheckVkResult(vkCreateWin32SurfaceKHR(instance, &win32SurfaceCreateInfo, nullptr, &surface), "error creating window surface");
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
	std::vector<std::vector<uint32_t>> _queueFamilyIndices(pDeviceCount);
	vkEnumeratePhysicalDevices(instance, &pDeviceCount, &pDevices[0]);

	if (pDeviceCount == 0) {
		throw std::runtime_error("No Vulkan compatible gpu has been found");
	}

#ifndef NDEBUG
	std::cout << "Enumerating physical devices" << std::endl;
#endif

	std::vector<int> scores;
	for (uint32_t i = 0; i < pDevices.size(); i++) {
#ifndef NDEBUG
		VkPhysicalDeviceProperties pProperties;
		vkGetPhysicalDeviceProperties(pDevices[i], &pProperties);
		std::cout << "Found " << pProperties.deviceName << std::endl;
#endif
		scores.push_back(PhysicalDeviceScore(_queueFamilyIndices, i, pDevices[i]));
	}

	int highScore = -1;
	if (scores.size() > 1) {
		for (uint32_t i = 1; i < static_cast<uint32_t>(scores.size()); i++) {
			if (scores[i] > scores[i - 1]) { 
				physicalDevice = pDevices[i]; 
				highScore = scores[i]; 
				PushAllQueueFamilyIndices(_queueFamilyIndices, i);
			}
			else { 
				physicalDevice = pDevices[i - 1];  
				highScore = scores[i - 1]; 
				PushAllQueueFamilyIndices(_queueFamilyIndices, i-1);
			}
		}
	}
	else {
		highScore = scores[0]; 
		physicalDevice = pDevices[0];
		PushAllQueueFamilyIndices(_queueFamilyIndices, 0);
	}
	if (highScore < 0) {
		throw std::runtime_error("No gpu with requested vulkan features/extensions has been found");
	}

#ifndef NDEBUG
	{
		VkPhysicalDeviceProperties chosenDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &chosenDeviceProperties);
		std::cout << "Using " << chosenDeviceProperties.deviceName << std::endl;
	}
#endif

}

void VulkanHandler::PushAllQueueFamilyIndices(const std::vector<std::vector<uint32_t>> _queueFamilyIndices, const uint32_t& pDeviceIndex) {
	size_t s = _queueFamilyIndices[pDeviceIndex].size();
	for (const uint32_t &index : _queueFamilyIndices[pDeviceIndex]) {
		queueFamilyIndices.insert(index);
	}
}

int VulkanHandler::PhysicalDeviceScore(std::vector<std::vector<uint32_t>> _queueFamilyIndices, const uint32_t& pDeviceIndex, const VkPhysicalDevice &pDevice) {
	
	int score = -1;

	if (CheckQueueFamiliesSupport(_queueFamilyIndices, pDeviceIndex, pDevice) && CheckPhysicalDeviceExtensions(pDevice)) {
		VkPhysicalDeviceFeatures pFeatures;
		vkGetPhysicalDeviceFeatures(pDevice, &pFeatures);

		//RAW SCORE SYSTEM
		score += pFeatures.variableMultisampleRate * 16;
		score += pFeatures.shaderClipDistance;
		score += pFeatures.geometryShader * 20;

		VkPhysicalDeviceMemoryProperties pMemProperties;
		vkGetPhysicalDeviceMemoryProperties(pDevice, &pMemProperties);

		score += pMemProperties.memoryTypeCount;
		score += pMemProperties.memoryHeapCount;
	}
	return score;
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

bool VulkanHandler::CheckQueueFamiliesSupport(std::vector<std::vector<uint32_t>> _queueFamilyIndices, const uint32_t& pDeviceIndex, const VkPhysicalDevice &pDevice) {
	uint32_t queueFamilyCount;
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamiliesProperties(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(pDevice, &queueFamilyCount, &queueFamiliesProperties[0]);

#ifndef NDEBUG
	{
		VkPhysicalDeviceProperties pDeviceProperties;
		vkGetPhysicalDeviceProperties(pDevice, &pDeviceProperties);
		std::cout << "Enumerating queue families for " << pDeviceProperties.deviceName << std::endl;
	}
#endif
	size_t requiredQueueFlagsCount = requiredQueueFlags.size();
	uint32_t _queueFamilyIndicesCount = 0;
	bool presentSupport = 0;
	for (uint32_t i = 0; i < static_cast<uint32_t>(queueFamiliesProperties.size()); i++) {
#ifndef NDEBUG
		{
			const char* queueFlagName = TranslateQueueFlags(queueFamiliesProperties[i].queueFlags);
			std::cout << queueFlagName << std::endl;
		}
#endif
		for (const VkQueueFlags &queueFlag : requiredQueueFlags) {
			if (queueFamiliesProperties[i].queueFlags & queueFlag) {
				requiredQueueFlagsCount += -1;
				_queueFamilyIndices[pDeviceIndex].push_back(i);
				_queueFamilyIndicesCount++;
			}
			if (CheckPresentSupport(pDevice, i)) {
				_queueFamilyIndices[pDeviceIndex].push_back(i);
				presentSupport = 1;
			}
		}
	}
	
	return requiredQueueFlagsCount == 0 && presentSupport == 1;
}

bool VulkanHandler::CheckPresentSupport(const VkPhysicalDevice& pDevice, uint32_t queueFamilyIndex) {
	VkBool32 supported = 0;
	vkGetPhysicalDeviceSurfaceSupportKHR(pDevice, queueFamilyIndex, surface, &supported);
	return static_cast<bool>(supported);
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

	CheckVkResult(vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device), "Error creating logical device");

	for (const uint32_t& index : queueFamilyIndices) {
		VkCommandPool cmdPool = CreateCommandPool(index);
		CreateCmdBuffer(cmdPool);
		cmdPools.push_back(cmdPool);
	}
}

VkCommandPool VulkanHandler::CreateCommandPool(uint32_t queueFamilyIndex) {

	VkCommandPoolCreateInfo cmdPoolCreateInfo;
	cmdPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
	cmdPoolCreateInfo.queueFamilyIndex = queueFamilyIndex;
	cmdPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
	cmdPoolCreateInfo.pNext = VK_NULL_HANDLE;

#ifndef NDEBUG
	std::cout << "Creating " << TranslateQueueFlags(requiredQueueFlags[queueFamilyIndex]) << " command pool" << std::endl;
#endif

	VkCommandPool cmdPool = VK_NULL_HANDLE;
	CheckVkResult(vkCreateCommandPool(device, &cmdPoolCreateInfo, nullptr, &cmdPool), "Error creating command pool");

	return cmdPool;
}

void VulkanHandler::CreateCmdBuffer(const VkCommandPool &cmdPool) {
	VkCommandBufferAllocateInfo cmdBufferAllocateInfo{};
	cmdBufferAllocateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
	cmdBufferAllocateInfo.pNext = VK_NULL_HANDLE;
	cmdBufferAllocateInfo.commandPool = cmdPool;
	cmdBufferAllocateInfo.commandBufferCount = 1;
	cmdBufferAllocateInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
	
#ifndef NDEBUG
	std::cout << "Creating command buffer" << std::endl;
#endif
	VkCommandBuffer cmdBuffer;
	CheckVkResult(vkAllocateCommandBuffers(device, &cmdBufferAllocateInfo, &cmdBuffer), "Error creating command buffer");
}

void VulkanHandler::CreateSwapchain() {

	VkSurfaceCapabilitiesKHR surfaceCapabilities;
	CheckVkResult(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &surfaceCapabilities), "error getting surface capabilities");
#ifndef NDEBUG
	std::cout << "Surface capabilities: " << std::endl;
	std::cout << "current extent: " << surfaceCapabilities.currentExtent.width << ", " << surfaceCapabilities.currentExtent.height << std::endl;
	std::cout << "current transform: " << surfaceCapabilities.currentTransform << std::endl;
	std::cout << "max image array layers: " << surfaceCapabilities.maxImageArrayLayers << std::endl;
	std::cout << "max image count: " << surfaceCapabilities.maxImageCount << std::endl;
	std::cout << "min image count: " << surfaceCapabilities.minImageCount << std::endl;
	std::cout << "supported composite alpha: " << surfaceCapabilities.supportedCompositeAlpha << std::endl;
	std::cout << "supported transforms: " << surfaceCapabilities.supportedTransforms << std::endl;
	std::cout << "supported usage flags: " << surfaceCapabilities.supportedUsageFlags << std::endl;
#endif

	uint32_t surfaceFormatsCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, nullptr);
	std::vector<VkSurfaceFormatKHR> surfaceFormats(surfaceFormatsCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &surfaceFormatsCount, &surfaceFormats[0]);
#ifndef NDEBUG
	std::cout << "Surface formats: " << std::endl;
	for (uint32_t i = 0; i < surfaceFormatsCount; i++) {
		std::cout << i << ": " << std::endl;
		std::cout << "color space: " << surfaceFormats[i].colorSpace << std::endl;
		std::cout << "format: " << surfaceFormats[i].format << std::endl;
	}
#endif

	uint32_t presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);
	std::vector<VkPresentModeKHR> presentModes(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, &presentModes[0]);
#ifndef NDEBUG
	std::cout << "Present modes: " << std::endl;
	for (uint32_t i = 0; i < presentModeCount; i++) {
		std::cout << presentModes[i] << std::endl;
	}
#endif



	VkSwapchainCreateInfoKHR swapchainCreateInfo{};
	swapchainCreateInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
	swapchainCreateInfo.pNext = VK_NULL_HANDLE;
	swapchainCreateInfo.surface = surface;
	swapchainCreateInfo.minImageCount = surfaceCapabilities.minImageCount;
	swapchainCreateInfo.imageFormat;
	swapchainCreateInfo.imageExtent.width = surfaceCapabilities.currentExtent.width;
	swapchainCreateInfo.imageExtent.height = surfaceCapabilities.currentExtent.height;
	swapchainCreateInfo.preTransform;
	swapchainCreateInfo.compositeAlpha;
	swapchainCreateInfo.imageArrayLayers = 1;
	swapchainCreateInfo.presentMode = presentModes[0];
	swapchainCreateInfo.clipped = 1;
	swapchainCreateInfo.imageColorSpace = VK_COLORSPACE_SRGB_NONLINEAR_KHR;
	swapchainCreateInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
	
	swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	swapchainCreateInfo.queueFamilyIndexCount;
	swapchainCreateInfo.pQueueFamilyIndices;
}

void VulkanHandler::Cleanup() {
	vkDestroyInstance(instance, nullptr);
	vkDestroyDevice(device, nullptr);
	for (const VkCommandPool& cmdPool : cmdPools) {
		vkDestroyCommandPool(device, cmdPool, nullptr);
	}
}