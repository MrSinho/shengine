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

#define SH_UPDATE_PUSH_CONSTANT_PARAMETERS 1 << 0
#define SH_UPDATE_UNIFORM_PARAMETERS 1 << 1

typedef struct ShMaterialHost {
	ShVkFixedStates						fixed_states;
	ShVkPipeline						pipeline;
	uint32_t							entity_count;
	uint32_t							entities[SH_ECS_MAX_ENTITIES];
	ShMaterialClient					material_clients[SH_ECS_MAX_ENTITIES];
	ShMaterialHostExtensionStructures	extensions;
	void*								push_constant[128 / sizeof(void*)];
	uint32_t							uniform_total_size;
	void*								uniform_buffers[64000 / sizeof(void*) * 32];
} ShMaterialHost;

extern uint32_t shGetUniformOffset(ShMaterialHost* p_material, const uint32_t uniform_idx);

extern uint32_t shGetUniformTotalSize(ShMaterialHost* p_material);

extern uint8_t shEntityInMaterial(const uint32_t entity, ShMaterialHost* p_material);

typedef struct ShEngine ShEngine;

extern void shUpdateUniformParameters(ShEngine* p_engine, const uint32_t uniform_idx, ShMaterialHost* p_material);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_MATERIAL_H