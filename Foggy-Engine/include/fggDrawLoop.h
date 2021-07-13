#ifndef FGG_DRAW_LOOP_H
#define FGG_DRAW_LOOP_H

#include <vulkan/vulkan.h>



typedef struct FggVkCore FggVkCore;

typedef struct FggVkPipelineData FggVkPipelineData;

typedef struct FggMesh FggMesh;


extern void fggFrameReset(const FggVkCore core);

extern void fggFrameBegin(const FggVkCore core, uint32_t* swapchainImageIndex);

extern void fggPushConstants(const FggVkCore core, const FggVkPipelineData pipeData, VkShaderStageFlagBits shaderStage, const uint32_t size, const void* pPushConstants);

extern void fggBindPipeline(const FggVkCore core, const FggVkPipelineData pipeData);

extern void fggBindVertexBuffers(const FggVkCore core, const FggMesh mesh);

extern void fggDraw(const FggVkCore core, const FggVkPipelineData pipeData, const FggMesh mesh);

extern void fggFrameEnd(const FggVkCore core, const uint32_t swapchainImageIndex);

#endif