#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <array>

struct VulkanHandler {

	void InitVulkan();

#ifndef NDEBUG
	void EnableValidationLayers();
#endif

	void CreateInstance();
	void SetPhysicalDevice();
	void SetLogicalDevice();
	
	static int PhysicalDeviceScore(const VkPhysicalDevice &pDevice);
	static bool CheckQueueFamiliesSupport(const VkPhysicalDevice& pDevice);
	static const char* TranslateVkResult(const VkResult &vkResult);
	static const char* TranslateQueueFlags(const VkQueueFlags &queueFamilyProperties);

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	std::array<const char*, 1> requiredValidationLayers = {
		"VK_LAYER_KHRONOS_validation",
	};
};

#endif