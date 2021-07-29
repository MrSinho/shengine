#ifndef FGG_DRAW_LOOP_H
#define FGG_DRAW_LOOP_H

#include <vulkan/vulkan.h>



typedef struct FggVkCore FggVkCore;

typedef struct FggVkPipelineData FggVkPipelineData;

typedef struct FggMesh FggMesh;

typedef struct FggVkFixedStates FggVkFixedStates;

extern void fggFrameReset(const FggVkCore core);

extern void fggFrameBegin(const FggVkCore core, uint32_t* swapchainImageIndex);

extern void fggBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData);

extern void fggPushConstants(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData);

extern void fggBindVertexBuffers(const FggVkCore core, const FggMesh mesh);

extern void fggBindIndexBuffers(const FggVkCore core, const FggMesh mesh);

extern void fggBindDescriptorSets(const FggVkCore core, FggVkPipelineData pipeData);

extern void fggDraw(const VkCommandBuffer graphicsCmdBuffer, const FggVkFixedStates fStates, const FggMesh mesh);

extern void fggFrameEnd(const FggVkCore core, const uint32_t swapchainImageIndex);

#endif