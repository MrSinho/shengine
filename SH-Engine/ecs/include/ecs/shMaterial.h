#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#include "ecs/shEcsImplementation.h"

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkPipelineData.h>

typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX 		= 0x00000001,
    SH_SHADER_STAGE_GEOMETRY 	= 0x00000008,
    SH_SHADER_STAGE_FRAGMENT 	= 0x00000010
} ShShaderStageFlags;

typedef struct ShUniformBufferInfo {
	uint32_t				uniform_size;
	ShShaderStageFlags		uniform_stage;
} ShUniformBufferInfo;

typedef struct ShMaterialInfo {
	const char*				vertex_shader_path;
	const char*				fragment_shader_path;
	uint32_t				uniform_buffer_count;
	ShUniformBufferInfo*	p_uniform_buffers;
	uint32_t				pConstSize;
	ShShaderStageFlags		pConstStage;
	ShFixedStateFlags		fixed_states_flags;
} ShMaterialInfo;

typedef struct ShMaterial {
	ShMaterialInfo			material_info;
	ShVkFixedStates			fixed_states;
	ShVkPipelineData		pipeline_data;
} ShMaterial;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMaterial, 2)

static void shMaterialsRelease(uint32_t* p_mat_info_count, ShMaterial** pp_materials) {
	free(*pp_materials); *p_mat_info_count = 0;
}

extern void shSetupShaders(const ShVkCore core, const char* vertexspv, const char* fragmentspv, ShVkPipelineData* p_pipe_data);

extern void shSetupMaterial(const ShVkCore core, ShMaterial* p_material);

#endif//SH_MATERIAL_H