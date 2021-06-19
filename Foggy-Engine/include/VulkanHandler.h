#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <stdint.h>

struct OpenGLHandler {

	void Init(uint32_t width, uint32_t height, const char *title);

	void CreateInstance();
	void CreateWindowSurface();
	void SetPhysicalDevice();
	bool CheckPhysicalDeviceExtensions(const VkPhysicalDevice &pDevice);
	VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex);
	void SetLogicalDevice();
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex);
	void CreateCmdBuffer(const VkCommandPool &cmdPool);
	void CreateSwapchain();
	void CreateDepthBuffer();
	void Cleanup();

// -----------------------------------------------------------------------------------------------------------------------------------------------------

	Window window;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSwapchainKHR swapchain;

#ifndef NDEBUG
	std::array<const char*, 1> requiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#endif

	std::array<const char*, 1> requiredDeviceExtensionsNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	const uint32_t requiredQueueFlagsCount = 1;

	std::array<VkQueueFlags, 1> requiredQueueFlags = { VK_QUEUE_GRAPHICS_BIT };

	std::vector<uint32_t> queueFamilyIndices;

	uint32_t presentQueueFamilyIndex;

	std::vector<VkCommandPool> cmdPools;
};

#endif