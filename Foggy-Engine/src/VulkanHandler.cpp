#include <stdexcept>
#include <vector>
#include <string>

#ifndef NDEBUG
	#include <iostream>
#endif

#include "VulkanHandler.h"

void VulkanHandler::InitVulkan() {
#ifndef NDEBUG
	EnableValidationLayers();
#endif
	CreateInstance();
	SetPhysicalDevice();
	SetLogicalDevice();
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
	//createInfo.enabledExtensionCount = ;
#endif
	VkResult result = vkCreateInstance(&createInfo, nullptr, &instance);
	if (result != VK_SUCCESS) {
#ifndef NDEBUG
		std::cout << "" << std::endl;
#endif	
		throw std::runtime_error((std::string("Error creating vkinstance: ") + std::string(TranslateVkResult(result))).c_str());
	}
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
}

#ifndef NDEBUG

void VulkanHandler::EnableValidationLayers() {
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, &availableLayers[0]);

	size_t requestedLayers = requiredValidationLayers.size();

	std::cout << "Installed validation layers: " << std::endl;
	for (uint32_t i = 0; i < requiredValidationLayers.size(); i++) {
		for (const VkLayerProperties& properties : availableLayers) {
			std::cout << properties.layerName << " - VERSION " << properties.implementationVersion <<
				": " << properties.description << std::endl;
			if (!strcmp(properties.layerName, requiredValidationLayers[i])) {
				requestedLayers -= 1;
			}
		}
	}
	if (requestedLayers > 0) {
		throw std::runtime_error("Not all the required validation layers have been found");
	}
}

#endif

void VulkanHandler::SetPhysicalDevice() {

	uint32_t pDeviceCount;
	vkEnumeratePhysicalDevices(instance, &pDeviceCount, nullptr);

	std::vector<VkPhysicalDevice> pDevices(pDeviceCount);
	vkEnumeratePhysicalDevices(instance, &pDeviceCount, &pDevices[0]);

	if (pDeviceCount == 0) {
		throw std::runtime_error("No Vulkan compatible gpu has been found");
	}

#ifndef NDEBUG
	std::cout << "Enumerating physical devices" << std::endl;
#endif

	std::vector<int> scores;
	for (const VkPhysicalDevice& pDevice : pDevices) {
#ifndef NDEBUG
		VkPhysicalDeviceProperties pProperties;
		vkGetPhysicalDeviceProperties(pDevice, &pProperties);
		std::cout << "Found " << pProperties.deviceName << std::endl;
#endif

		scores.push_back(PhysicalDeviceScore(pDevice));
	}

	int highScore = -1;
	if (scores.size() > 1) {
		for (uint32_t i = 1; i < scores.size(); i++) {
			if (scores[i] > scores[i - 1]) { physicalDevice = pDevices[i]; highScore = scores[i]; }
			else { physicalDevice = pDevices[i - 1];  highScore = scores[i - 1]; }
		}
	}
	else {
		highScore = scores[0]; physicalDevice = pDevices[0];
	}
	
	if (highScore < 0) {
		throw std::runtime_error("No gpu with requested vulkan features has been found");
	}

#ifndef NDEBUG
	{
		VkPhysicalDeviceProperties chosenDeviceProperties;
		vkGetPhysicalDeviceProperties(physicalDevice, &chosenDeviceProperties);
		std::cout << "Using " << chosenDeviceProperties.deviceName << std::endl;
	}
#endif

}

int VulkanHandler::PhysicalDeviceScore(const VkPhysicalDevice &pDevice) {
	
	int score = -1;

	if (!CheckQueueFamiliesSupport(pDevice)) {
		return score;
	}

	VkPhysicalDeviceFeatures pFeatures;
	vkGetPhysicalDeviceFeatures(pDevice, &pFeatures);

	//RAW SCORE SYSTEM
	score += pFeatures.variableMultisampleRate * 16;
	score += pFeatures.shaderClipDistance;
	score += pFeatures.geometryShader * 20;

	return score;
}

bool VulkanHandler::CheckQueueFamiliesSupport(const VkPhysicalDevice &pDevice) {
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
	for (const VkQueueFamilyProperties &queueProperties : queueFamiliesProperties) {
#ifndef NDEBUG
		{
			const char* queueFlagName = TranslateQueueFlags(queueProperties.queueFlags);
			std::cout << queueFlagName << std::endl;
		}
#endif
		if (queueProperties.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
			return 1;
		}
	}
	return 0;
}

const char* VulkanHandler::TranslateQueueFlags(const VkQueueFlags& queueFlags) {
		if(queueFlags & VK_QUEUE_GRAPHICS_BIT)			{ return "VK_QUEUE_GRAPHICS_BIT";		}
		if(queueFlags & VK_QUEUE_COMPUTE_BIT)			{ return "VK_QUEUE_COMPUTE_BIT";		}
		if(queueFlags & VK_QUEUE_TRANSFER_BIT)			{ return "VK_QUEUE_TRANSFER_BIT";		}
		if(queueFlags & VK_QUEUE_SPARSE_BINDING_BIT)	{ return "VK_QUEUE_SPARSE_BINDING_BIT";	}
		if(queueFlags & VK_QUEUE_PROTECTED_BIT)			{ return "VK_QUEUE_PROTECTED_BIT";		}
#ifdef VK_ENABLE_BETA_EXTENSIONS
		if (queueFlags & VK_QUEUE_VIDEO_DECODE_BIT_KHR) { return "VK_QUEUE_VIDEO_DECODE_BIT_KHR"; }
#endif											
#ifdef VK_ENABLE_BETA_EXTENSIONS				
		if (queueFlags & VK_QUEUE_VIDEO_ENCODE_BIT_KHR) { return "VK_QUEUE_VIDEO_ENCODE_BIT_KHR"; }
#endif											
		if (queueFlags & VK_QUEUE_FLAG_BITS_MAX_ENUM)	{ return "VK_QUEUE_FLAG_BITS_MAX_ENUM"; }
	return "unknown flag";
}

void VulkanHandler::SetLogicalDevice() {
	
	VkDeviceCreateInfo deviceCreateInfo{};
	deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
	deviceCreateInfo.enabledLayerCount = static_cast<uint32_t>(requiredValidationLayers.size());
	deviceCreateInfo.ppEnabledLayerNames = &requiredValidationLayers[0];
	//deviceCreateInfo.queueCreateInfoCount = ;
	//deviceCreateInfo.pQueueCreateInfos = ;

	vkCreateDevice(physicalDevice, &deviceCreateInfo, nullptr, &device);

}
