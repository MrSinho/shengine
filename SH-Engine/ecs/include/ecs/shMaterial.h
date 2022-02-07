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

#endif//SH_MATERIAL_H