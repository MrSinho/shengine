#ifndef VULKAN_HANDLER_H
#define VULKAN_HANDLER_H

#include <vulkan/vulkan.h>
#include <stdint.h>

#include "Window.h"

typedef struct VkData {
	
	VkInstance instance;
	VkPhysicalDevice physicalDevice;
	VkDevice device;

	VkQueueFlags requiredQueueFlag;

	uint32_t graphicsQueueIndex;
	uint32_t presentQueueIndex;

	Window window;
	VkSurfaceKHR surface;

	VkQueue computeQueue;

	VkCommandPool *cmdPools;
	uint32_t cmdPoolCount;

	VkCommandBuffer *cmdBuffers;
	uint32_t cmdBufferCount;

	VkShaderModule *shaderModules;
	uint32_t shaderModuleCount;

} VkData;

/*
* First setup
*/
extern VkData VKDataInitPrerequisites(uint32_t width, uint32_t height, const char* title);
void InitVulkan(VkData* vkComputeData);
extern void CreateInstance(VkData* vkComputeData);
extern void CreateWindowSurface(const VkInstance instance, const GLFWwindow* window, VkSurfaceKHR* surface);
extern void SetPhysicalDevice(VkData* vkComputeData);
extern int CheckPhysicalDeviceExtensions(const VkData vkComputeData, const VkPhysicalDevice pDevice);

/*
*	Logical device creation + command buffers
*/

extern VkDeviceQueueCreateInfo CreateQueue(const uint32_t queueFamilyIndex, const float* priority);
extern void SetLogicalDevice(VkData* vkComputeData);
extern VkCommandPool CreateCommandPool(const VkDevice device, uint32_t queueFamilyIndex);
extern VkCommandBuffer CreateCmdBuffer(const VkDevice device, const VkCommandPool cmdPool);

/*
* 
*/
extern VkShaderModule CreateShaderModule(const VkDevice device, const char* input, const char* output);

/*
*	Utilities
*/

extern int CheckValidationLayer(const char *validationLayer);
extern const char* TranslateVkResult(const VkResult vkResult);
extern const char* TranslateQueueFlags(const VkQueueFlags queueFlag);
extern void CheckVkResult(VkResult result, const char* errormsg);
extern void Compile_glslc_Shader(const char* input, const char* output);
extern void Cleanup(VkData *vkComputeData);

#endif