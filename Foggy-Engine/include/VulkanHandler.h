#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#ifdef WIN32
	#define VK_USE_PLATFORM_WIN32_KHR
#endif

#ifdef __linux__
	#define VK_USE_PLATFORM_XLIB_KHR
#endif

#include <vulkan/vulkan.h>
#include <vector>
#include <array>
#include <set>
#include <stdint.h>
#include <string>

#include "Window.h"

typedef struct VulkanHandler {
	Window window;
	VkInstance instance = VK_NULL_HANDLE;
	VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
	VkDevice device = VK_NULL_HANDLE;
	VkSurfaceKHR surface = VK_NULL_HANDLE;
	VkSwapchainKHR swapchain = VK_NULL_HANDLE;
	VkFormat swapchainImageFormat;
	VkPipelineLayout pipelineLayout;
	VkRenderPass renderPass;
	std::array<const char*, 1> requiredValidationLayers = { "VK_LAYER_KHRONOS_validation" };
	std::array<const char*, 1> requiredDeviceExtensionsNames = { VK_KHR_SWAPCHAIN_EXTENSION_NAME };
	std::array<VkQueueFlags, 1> requiredQueueFlags = { VK_QUEUE_GRAPHICS_BIT };
	std::vector<uint32_t> queueFamilyIndices;
	std::vector<VkCommandPool> cmdPools;
	std::vector<VkImage> swapchainImages;
	std::vector<VkImageView> swapchainImageViews;
	std::vector<VkShaderModule> shaderModules;
	std::array<VkDynamicState, 2> dynamicStates = {
		VK_DYNAMIC_STATE_VIEWPORT,
		VK_DYNAMIC_STATE_LINE_WIDTH
	};
} VulkanHandler;

/*
* First setup
*/

void InitVulkan(VulkanHandler *vulkanHandler);
extern void CheckValidationLayers(const VulkanHandler &vulkanHandler);
extern void CreateInstance(VulkanHandler* vulkanHandler);
extern void CreateWindowSurface(const VkInstance &instance, const Window &window, VkSurfaceKHR *surface);
extern void SetPhysicalDevice(VulkanHandler* vulkanHandler);
extern bool CheckPhysicalDeviceExtensions(const VulkanHandler& vulkanHandler, const VkPhysicalDevice& pDevice);

/*
*	Logical device creation + command buffers
*/

extern VkDeviceQueueCreateInfo CreateQueue(uint32_t queueFamilyIndex, const float &priority);
extern void SetLogicalDevice(VulkanHandler* vulkanHandler);
extern VkCommandPool CreateCommandPool(const VkDevice &device, uint32_t queueFamilyIndex);
extern void CreateCmdBuffer(const VkDevice& device, const VkCommandPool& cmdPool);

/*
*	Swapchain creation + image views
*/

extern void CreateSwapchain(VulkanHandler *vulkanHandler);
extern void GetSwapchainImages(VulkanHandler* vulkanHandler);
extern void CreateSwapchainImageViews(VulkanHandler* vulkanHandler);

extern VkShaderModule CreateShaderModule(const VkDevice &device, const char* input, const char* output);

/*
*	Pipeline Stuff
*/

extern VkPipelineViewportStateCreateInfo SetViewportState(const VkPhysicalDevice& physicalDevice, const VkSurfaceKHR& surface);
extern VkPipelineRasterizationStateCreateInfo CreateRasterizer();
extern VkPipelineMultisampleStateCreateInfo EnableMSAA();
extern VkPipelineColorBlendStateCreateInfo ColorBlendSettings();
extern VkPipelineDynamicStateCreateInfo SetDynamicState(const VkPipelineDynamicStateCreateInfo& dynamicStates);
extern VkPipelineLayout SetPipelineLayout(const VkDevice& device);
extern VkRenderPass CreateRenderPass(const VkFormat& swapchainImageFormat, const VkDevice device, VkRenderPass* renderPass);
extern void CreateGraphicsPipeline(VulkanHandler& vulkanHandler);

/*
*	Utilities
*/

extern const char* TranslateVkResult(const VkResult& vkResult);
extern const char* TranslateQueueFlags(const VkQueueFlags& queueFlag);
extern void CheckVkResult(VkResult result, const char* errormsg);
extern void Compile_glslc_Shader(const char* input, const char* output);
extern void Cleanup(VulkanHandler *vulkanHandler);

#endif