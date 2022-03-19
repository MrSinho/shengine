#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "ecs/shEcsImplementation.h"

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkMemoryInfo.h>

typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX 		= VK_SHADER_STAGE_VERTEX_BIT,
    SH_SHADER_STAGE_FRAGMENT 	= VK_SHADER_STAGE_FRAGMENT_BIT
} ShShaderStageFlags;

typedef struct ShMaterialClient {
	void*		p_push_constant_parameters;
	void*		p_uniform_parameters;
} ShMaterialClient;

typedef struct ShMaterialHost {
	ShVkFixedStates			fixed_states;
	ShVkGraphicsPipeline	pipeline;
	uint32_t				entity_count;
	uint32_t				entities[SH_ECS_MAX_ENTITIES];
	ShMaterialClient		material_clients[SH_ECS_MAX_ENTITIES];
} ShMaterialHost;

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_MATERIAL_H