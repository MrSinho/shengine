#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shecs/shEcsImplementation.h"

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkMemoryInfo.h>

typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX 		= VK_SHADER_STAGE_VERTEX_BIT,
    SH_SHADER_STAGE_FRAGMENT 	= VK_SHADER_STAGE_FRAGMENT_BIT
} ShShaderStageFlags;

typedef struct ShMaterialClient {
	uint8_t		parameters;
	void*		p_push_constant_parameters[128/sizeof(void*)];
	void*		p_uniform_parameters[64000/sizeof(void*)];
} ShMaterialClient;

typedef struct ShMaterialHostExtensionStructures {
	uint32_t transform_uniform_offset;
	//other stuff like phyisics inputs
} ShMaterialHostExtensionStructures;

typedef struct ShMaterialHost {
	ShVkFixedStates						fixed_states;
	ShVkGraphicsPipeline				pipeline;
	uint32_t							entity_count;
	uint32_t							entities[SH_ECS_MAX_ENTITIES];
	ShMaterialClient					material_clients[SH_ECS_MAX_ENTITIES];
	ShMaterialHostExtensionStructures	extensions;
	void*								push_constant[128 / sizeof(void*)];
	void*								uniform_buffers[64000 / sizeof(void*) * 32];

} ShMaterialHost;

static uint32_t shGetUniformOffset(ShMaterialHost* p_material, const uint32_t uniform_idx) {
	uint32_t uniform_offset = 0;
	for (uint32_t i = 0; i < p_material->pipeline.uniform_count; i++) {
		if (i == uniform_idx) {
			return uniform_offset;
}
		uniform_offset += p_material->pipeline.uniform_buffers_size[i];
	}
	return uniform_offset;
}

static uint32_t shGetUniformTotalSize(ShMaterialHost* p_material) {
	uint32_t uniform_total_size = 0;
	for (uint32_t i = 0; i < p_material->pipeline.uniform_count; i++) {
		uniform_total_size += p_material->pipeline.uniform_buffers_size[i];
	}
	return uniform_total_size;
}

static uint8_t shEntityInMaterial(const uint32_t entity, ShMaterialHost* p_material) {
	for (uint32_t i = 0; i < p_material->entity_count; i++) {
		if (entity == p_material->entities[i]) { return 1; }
	}
	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_MATERIAL_H