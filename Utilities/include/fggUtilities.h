#ifndef FGG_UTILITIES_H
#define FGG_UTILITIES_H

#include <stdint.h>
#include <vulkan/vulkan.h>

extern int fggCheckValidationLayer(const char* validationLayer);

extern const char* fggTranslateVkResult(const VkResult vkResult);

extern void fggCheckVkResult(VkResult result, const char* errormsg);

extern void fggCheckValue(const int result, const int desiredValue, const char *errormsg);

extern void fggBuildShader(const char* input, const char* output);

extern const char *fggReadCode(const char *path, uint32_t *pCodeSize, const char *mode);

#endif