#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <vector>
#include <array>

#include "Window.h"

struct VulkanHandler {

	void InitVulkan();

#ifndef NDEBUG
	void EnableValidationLayers();
#endif

	void CreateInstance();
	void SetPhysicalDevice();
	bool CheckQueueFamiliesSupport(const VkPhysicalDevice& pDevice);
	bool CheckPhysicalDeviceExtensions(const VkPhysicalDevice &pDevice);
	VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex);
	void SetLogicalDevice();
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex);

	int PhysicalDeviceScore(const VkPhysicalDevice& pDevice);
	static const char* TranslateVkResult(const VkResult &vkResult);
	static const char* TranslateQueueFlags(const VkQueueFlags &queueFlag);

	void Cleanup();

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	
#ifndef NDEBUG
	std::array<const char*, 1> requiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#endif
	std::array<const char*, 1> requiredExtensionsNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	std::array<VkQueueFlags, 1> requiredQueueFlags = { VK_QUEUE_GRAPHICS_BIT };
	std::vector<uint32_t> queueFamiliesIndices;
	std::vector<VkCommandPool> cmdPools;
};

#endif