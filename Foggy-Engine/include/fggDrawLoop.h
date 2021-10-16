#ifndef FGG_DRAW_LOOP_H
#define FGG_DRAW_LOOP_H

#include <vulkan/vulkan.h>



typedef struct FggVkCore FggVkCore;

typedef struct FggVkPipelineData FggVkPipelineData;

typedef struct FggMeshInfo FggMeshInfo;

typedef struct FggMesh FggMesh;

typedef struct FggVkFixedStates FggVkFixedStates;



extern void fggFrameReset(const FggVkCore core);

extern void fggFrameBegin(const FggVkCore core, uint32_t* swapchainImageIndex);

extern void fggBindPipeline(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData);

extern void fggPushConstants(const VkCommandBuffer graphicsCmdBuffer, const FggVkPipelineData pipeData, const void* pPushConstantsData);

extern void fggBindVertexBuffers(const FggVkCore core, const FggMesh mesh);

extern void fggBindIndexBuffers(const FggVkCore core, const FggMesh mesh);

extern void fggBindDescriptorSets(const FggVkCore core, FggVkPipelineData pipeData);

extern void fggDraw(const VkCommandBuffer graphicsCmdBuffer, const uint32_t count, const uint32_t stride, const FggMesh mesh);

extern void fggFrameEnd(const FggVkCore core, const uint32_t swapchainImageIndex);


extern void fggRenderMesh(const FggVkCore core, const FggVkPipelineData pipe_data, const uint32_t push_const_size, void* p_push_const, const uint32_t uniform_size, void* p_uniform, const FggMeshInfo mesh_info, FggMesh* mesh);

#endif