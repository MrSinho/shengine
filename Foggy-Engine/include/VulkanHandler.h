#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#ifdef WIN32
#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_EXPOSE_NATIVE_WIN32
#endif
#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <set>
#include <stdint.h>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include "Window.h"

struct VulkanHandler {

	void InitVulkan(uint32_t width, uint32_t height, const char* title);

#ifndef NDEBUG
	void EnableValidationLayers();
#endif

	void CreateInstance();
	
	void CreateWindowSurface();
	
	void SetPhysicalDevice();
	bool CheckPhysicalDeviceExtensions(const VkPhysicalDevice& pDevice);
	
	VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex);
	
	void SetLogicalDevice();
	
	VkCommandPool CreateCommandPool(uint32_t queueFamilyIndex);
	void CreateCmdBuffer(const VkCommandPool& cmdPool);
	
	void CreateSwapchain();
	
	void GetSwapchainImages();
	void CreateSwapchainImageViews();
	
	VkShaderModule CreateShaderModule(const char* input, const char* output);

	VkPipelineViewportStateCreateInfo SetViewportState();
	VkPipelineRasterizationStateCreateInfo CreateRasterizer();
	VkPipelineMultisampleStateCreateInfo EnableMSAA();
	VkPipelineColorBlendStateCreateInfo ColorBlendSettings();
	VkPipelineDynamicStateCreateInfo SetDynamicState();
	VkPipelineLayout SetPipelineLayout();
	VkRenderPass CreateRenderPass();
	void CreateGraphicsPipeline();

	static const char* TranslateVkResult(const VkResult& vkResult);
	static const char* TranslateQueueFlags(const VkQueueFlags& queueFlag);
	static void CheckVkResult(VkResult result, const char* errormsg);
	static void CompileShader(const char * input, const char* output);
	void Cleanup();

	// -----------------------------------------------------------------------------------------------------------------------------------------------------

	Window window;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkFormat swapchainImageFormat;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;

#ifndef NDEBUG
	std::array<const char*, 1> requiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
#endif

	std::array<const char*, 1> requiredDeviceExtensionsNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };

	const uint32_t requiredQueueFlagsCount = 1;

	std::array<VkQueueFlags, 1> requiredQueueFlags = { VK_QUEUE_GRAPHICS_BIT };

	std::vector<uint32_t> queueFamilyIndices;

	uint32_t presentQueueFamilyIndex;

	std::vector<VkCommandPool> cmdPools;

	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;

	std::vector<VkShaderModule> shaderModules;

	std::array<VkDynamicState, 2> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
};

#endif