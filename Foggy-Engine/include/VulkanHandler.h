#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>

struct VulkanHandler {

	void InitVulkan();

	void CreateInstance();
	void SetPhysicalDevice();
	void SetLogicalDevice();
	
	int PhysicalDeviceScore(const VkPhysicalDevice &pDevice);
	
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	const char* requiredExtensions[0] = {
		
	};
};

#endif