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


#define REQUIRED_QUEUE_FLAGS { VK_QUEUE_GRAPHICS_BIT } // defines the required queue flags
#define REQUIRED_QUEUE_FLAGS_COUNT 1 //the number of required queue flags

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
	*	Checks if given gpu "has" the required queue families
	*	@param vector of arrays of queue family indices where the arrays count is equal to the physical devices count,
	*		   the current physical device index,
	*		   the current physical device
	*	@return true or false, depending if the physical device satisfies the required queue families.
	*/
	bool CheckQueueFamiliesSupport(	std::vector<std::array<uint32_t, REQUIRED_QUEUE_FLAGS_COUNT>> _queueFamilyIndices,
									const uint32_t &pDeviceIndex, 
									const VkPhysicalDevice& pDevice);
	/*
	*	Checks if the physical device queue supports presenting
	* 
	*/
	bool CheckPresentSupport(const VkPhysicalDevice &pDevice, uint32_t queueFamilyIndex);

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

// -----------------------------------------------------------------------------------------------------------------------------------------------------

	/*
	*	Sets a score for the gpu using a raw "scoring system"
	*	@param vector of arrays of queue family indices where the arrays count is equal to the physical devices count,
	*		   the current physical device index,
	*		   the current physical device
	*	@return the gpu score
	*/
	int PhysicalDeviceScore(std::vector<std::array<uint32_t, REQUIRED_QUEUE_FLAGS_COUNT>> _queueFamilyIndices,
							const uint32_t &pDeviceIndex,
							const VkPhysicalDevice& pDevice);

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
	*	Sets queueFamilyIndices array equal to the given array of the chosen gpu
	*	@param vector of arrays of queue family indices where the arrays count is equal to the physical devices count,
	*		   the current physical device index,
	*/
	void PushAllQueueFamilyIndices(const std::vector<std::array<uint32_t, REQUIRED_QUEUE_FLAGS_COUNT>> _queueFamilyIndices, const uint32_t &pDeviceIndex);

	/*
	*	Does a nice cleanup
	*/
	void Cleanup();

// -----------------------------------------------------------------------------------------------------------------------------------------------------

	Window window;

	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface;

	uint32_t physicalDeviceIndex;

#ifndef NDEBUG
	std::array<const char*, 1> requiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#endif
	std::array<const char*, 1> requiredDeviceExtensionsNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	const uint32_t requiredQueueFlagsCount = 1;
	std::array<VkQueueFlags, REQUIRED_QUEUE_FLAGS_COUNT> requiredQueueFlags = REQUIRED_QUEUE_FLAGS;
	std::array<uint32_t, REQUIRED_QUEUE_FLAGS_COUNT> queueFamiliesIndices;
	std::vector<VkCommandPool> cmdPools;
};

#endif