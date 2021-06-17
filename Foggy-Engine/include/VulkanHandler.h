#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#ifdef WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
	#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <stdint.h>

#define GLFW_INCLUDE_VULKAN
#include "Window.h"

struct VulkanHandler {

	void InitVulkan(uint32_t width, uint32_t height, const char *title);

#ifndef NDEBUG
	/*
	*	Enables validation layers
	*/
	void EnableValidationLayers();
#endif

	/*
	*	Creates an application info, 
	*	loops through instance extensions
	*	and creates a vkinstance
	*/
	void CreateInstance();

	/*
	*	Creates a valid window surface
	*/
	void CreateWindowSurface();

	/*
	*	Gets the hardware installed physical deivices,
	*	calls PhysicalDeviceScore to get the score for each device,
	*	sets the physical device and the queue family indices
	*/
	void SetPhysicalDevice();

	/*
	*	Checks if the given gpu supports the required extensions
	*	@param the current physical device
	*	@return true or false
	*/
	bool CheckPhysicalDeviceExtensions(const VkPhysicalDevice &pDevice);

	/*
	*	Creates a queue given the family index
	*	@param the queue family index
	*	@return the setup vkdevicequeuecreateinfo
	*/
	VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex);

	/*
	*	Creates the logical device
	*/
	void SetLogicalDevice();

	/*
	*	Creates a command pool given the queue family index
	*/
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex);

	/*
	*	Creates a command buffer given the command pool
	*	@param a successfully created command pool with a valid queue family index
	*/
	void CreateCmdBuffer(const VkCommandPool &cmdPool);

	/*
	*	Creates an ez swapchain
	*/
	void CreateSwapchain();

// -----------------------------------------------------------------------------------------------------------------------------------------------------

	/*
	*	@param the vkresult
	*	@return the corresponding c string of the vkresult
	*/
	static const char* TranslateVkResult(const VkResult &vkResult);

	/*
	*	@param the queue flag
	*	@return the corresponding c string of the queue flag
	*/
	static const char* TranslateQueueFlags(const VkQueueFlags &queueFlag);

	/*
	*	Checks if the given vkresult is equal to VK_SUCCESS, if not throw an error with the given message
	*	@param a valid vkresult, the error message in case vkresult is not equal to VK_SUCCESS
	*/
	static void CheckVkResult(VkResult result, const char *errormsg);

	/*
	*	
	*/
	VkSurfaceCapabilitiesKHR GetSurfaceCapabilities();

	/*
	*	Does a nice cleanup
	*/
	void Cleanup();

// -----------------------------------------------------------------------------------------------------------------------------------------------------

	Window window;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;

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