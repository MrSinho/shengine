#ifndef FGG_VULKAN_1_0_H
#define FGG_VULKAN_1_0_H

#include <vulkan/vulkan.h>

#define FGG_VULKAN_VERSION 12

#if FGG_VULKAN_VERSION == 10
	#define FGG_VULKAN_VERSION VK_API_VERSION_1_0
#elif FGG_VULKAN_VERSION == 11
	#define FGG_VULKAN_VERSION VK_API_VERSION_1_0
#endif

#endif