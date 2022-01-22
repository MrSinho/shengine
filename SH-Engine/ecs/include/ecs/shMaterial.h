#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#include "ecs/shEcsImplementation.h"

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkMemoryInfo.h>

typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX 		= 0x00000001,
    SH_SHADER_STAGE_GEOMETRY 	= 0x00000008,
    SH_SHADER_STAGE_FRAGMENT 	= 0x00000010
} ShShaderStageFlags;

typedef struct ShMaterialHost {
	ShVkFixedStates			fixed_states;
	ShVkGraphicsPipeline	pipeline;
	uint32_t				entity_count;
	uint32_t				entities[SH_ECS_MAX_ENTITIES];
} ShMaterialHost;

#include <assert.h>

static void shMaterialsRelease(ShVkCore* p_core, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials) {
	assert(p_mat_info_count != NULL && pp_materials != NULL);
	for (uint32_t i = 0; i < *p_mat_info_count; i++) {
		for (uint32_t j = 0; j < (*pp_materials)[i].pipeline.uniform_count; j++) {
			shClearUniformBufferMemory(p_core, j, &(*pp_materials)[i].pipeline);
		}
		shDestroyPipeline(p_core, &(*pp_materials)[i].pipeline);
	}
	free(*pp_materials); *p_mat_info_count = 0;
}

#endif//SH_MATERIAL_H