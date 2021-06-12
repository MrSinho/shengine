#include <stdexcept>
#include <vector>

#ifndef NDEBUG
	#include <iostream>
#endif

#include "VulkanHandler.h"

void VulkanHandler::InitVulkan() {
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
	//createInfo.ppEnabledExtensionNames = ;
	//createInfo.enabledExtensionCount = ;
	//createInfo.enabledLayerCount = ;

	if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
		throw std::runtime_error("Error creating vkinstance");
	}
}

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
		throw std::runtime_error("Your best gpu doesn't support the requested vulkan features...");
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

	VkPhysicalDeviceProperties pProperties;
	vkGetPhysicalDeviceProperties(pDevice, &pProperties);

#ifndef NDEBUG
	std::cout << "Found " << pProperties.deviceName << std::endl;
#endif

	VkPhysicalDeviceFeatures pFeatures;
	vkGetPhysicalDeviceFeatures(pDevice, &pFeatures);

	//RAW SCORE SYSTEM
	score += pFeatures.variableMultisampleRate * 16;
	score += pFeatures.shaderClipDistance;
	score += pFeatures.geometryShader * 20;

	return score;
}

void VulkanHandler::SetLogicalDevice() {

}
