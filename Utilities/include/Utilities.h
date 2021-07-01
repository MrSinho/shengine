#ifndef UTILITIES_H
#define UTILITIES_H

#include <stdint.h>
#include <vulkan/vulkan.h>

extern int CheckValidationLayer(const char* validationLayer);
extern const char* TranslateVkResult(const VkResult vkResult);
extern const char* TranslateQueueFlags(const VkQueueFlags queueFlag);
extern void CheckVkResult(VkResult result, const char* errormsg);
extern void BuildShader(const char* input, const char* output);
extern const char *ReadCode(const char *path, uint32_t *codeSize, const char *mode);

#endif