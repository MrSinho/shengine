#ifndef FGG_VULKAN_1_0_H
#define FGG_VULKAN_1_0_H

#include <vulkan/vulkan.h>

#ifdef FGG_USE_VULKAN_1_0
#define FGG_VULKAN_VERSION VK_API_VERSION_1_0
#elifdef FGG_USE_VULKAN_1_1
#define FGG_VULKAN_VERSION VK_API_VERSION_1_0
#else
#define FGG_VULKAN_VERSION VK_API_VERSION_1_2
#endif

#endif