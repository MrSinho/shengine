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
				queueFamilyIndices = suitableDevicesQueueFamilyIndices[i];
			}
			else {
				physicalDevice = suitableDevices[i - 1];
				queueFamilyIndices = suitableDevicesQueueFamilyIndices[i - 1];
			}
		}
	}
	else {
		physicalDevice = suitableDevices[0];
		queueFamilyIndices = suitableDevicesQueueFamilyIndices[0];
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
	uint32_t queueFamilyPropertiesCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, nullptr);
	std::vector<VkQueueFamilyProperties> queueFamilyProperties(queueFamilyPropertiesCount);
	vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyPropertiesCount, &queueFamilyProperties[0]);
	std::cout << "Creating " << TranslateQueueFlags(queueFamilyProperties[queueFamilyIndex].queueFlags) << " command pool" << std::endl;
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
	//if (queueFamilyIndices)
	//swapchainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	//swapchainCreateInfo.queueFamilyIndexCount;
	//swapchainCreateInfo.pQueueFamilyIndices;
}

void VulkanHandler::Cleanup() {
	vkDestroyInstance(instance, nullptr);
	vkDestroyDevice(device, nullptr);
	for (const VkCommandPool& cmdPool : cmdPools) {
		vkDestroyCommandPool(device, cmdPool, nullptr);
	}
}