#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#include "ecs/shEcsImplementation.h"

#include <shVkCore.h>
#include <shVkPipelineData.h>

typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX 		= 0x00000001,
    SH_SHADER_STAGE_GEOMETRY 	= 0x00000008,
    SH_SHADER_STAGE_FRAGMENT 	= 0x00000010
} ShShaderStageFlags;

typedef struct ShUniformBufferInfo {
	uint32_t				uniformSize;
	ShShaderStageFlags		uniformStage;
} ShUniformBufferInfo;

typedef struct ShMaterialInfo {
	const char* 			vertex_shader_path;
	const char* 			fragment_shader_path;	
	uint32_t				uniform_buffer_count;
	ShUniformBufferInfo*	p_uniform_buffers;
	uint32_t				pConstSize; 
	ShShaderStageFlags		pConstStage;
	ShFixedStateFlags		fixed_states_flags;
} ShMaterialInfo;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMaterialInfo, 5)

static void shMaterialInfosRelease(uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos) {
	free(*pp_mat_infos); *p_mat_info_count = 0;
}

typedef struct ShMaterial {
	ShMaterialInfo			mat_info;
	ShVkFixedStates			fixed_states;
	ShVkPipelineData		pipeline_data;
} ShMaterial;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMaterial, 2)

extern void shSetupShaders(const ShVkCore core, const char* vertexspv, const char* fragmentspv, ShVkPipelineData* p_pipe_data);

extern void shSetupMaterial(const ShVkCore core, const ShMaterialInfo mat_info, ShMaterial* p_material);

#endif//SH_MATERIAL_H