#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <vector>

#ifndef NDEBUG
	#define REQUIRED_VALIDATION_LAYERS { "VK_LAYER_KHRONOS_validation" }
	#define REQUIRED_VALIDATION_LAYERS_SIZE 1
#endif

#define REQUIRED_QUEUE_FLAGS { VK_QUEUE_GRAPHICS_BIT }
#define REQUIRED_QUEUE_FLAGS_SIZE 1

struct VulkanHandler {

	void InitVulkan();

#ifndef NDEBUG
	void EnableValidationLayers();
#endif

	void CreateInstance();
	void SetPhysicalDevice();
	bool CheckQueueFamiliesSupport(const VkPhysicalDevice& pDevice);
	void SetLogicalDevice();
	
	int PhysicalDeviceScore(const VkPhysicalDevice& pDevice);
	static const char* TranslateVkResult(const VkResult &vkResult);
	static const char* TranslateQueueFlags(const VkQueueFlags &queueFlag);

	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
#ifndef NDEBUG
	const char* requiredValidationLayers[REQUIRED_VALIDATION_LAYERS_SIZE] = REQUIRED_VALIDATION_LAYERS;
#endif
	VkQueueFlags requiredQueueFlags[REQUIRED_QUEUE_FLAGS_SIZE] = REQUIRED_QUEUE_FLAGS;
	std::vector<VkDeviceQueueCreateInfo> deviceQueueCreateInfos;
};

#endif