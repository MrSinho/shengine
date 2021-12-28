#ifndef SH_VKCHECK_H
#define SH_VKCHECK_H

#include <vulkan/vulkan.h>

#ifndef NDEBUG

extern int shCheckValidationLayer(const char* validationLayer);

#endif//NDEBUG

extern const char* shTranslateVkResult(const VkResult vkResult);

extern void shCheckVkResult(VkResult result, const char* errormsg);


#endif//SH_VKCHECK_H