#ifndef SH_MATERIAL_INFO_H
#define SH_MATERIAL_INFO_H

#include "shEcsImplementation.h"

typedef enum ShFixedStateFlags {
	SH_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT = 0x01,
	SH_FIXED_STATES_POLYGON_MODE_FACE_BIT = 0x02,
	SH_FIXED_STATES_POLYGON_MODE_POINTS_BIT = 0x04,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 0x08,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST = 0x10,
	SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST = 0x20,
	SH_FIXED_STATES_VERTEX_POSITIONS_BIT = 0x40,
	SH_FIXED_STATES_VERTEX_NORMALS_BIT = 0x80,
	SH_FIXED_STATES_VERTEX_TCOORDS_BIT = 0x100
} ShFixedStateFlags;



typedef enum ShShaderStageFlags {
	SH_SHADER_STAGE_VERTEX_BIT 	= 0x00000001,
    SH_SHADER_STAGE_GEOMETRY_BIT 	= 0x00000008,
    SH_SHADER_STAGE_FRAGMENT_BIT 	= 0x00000010
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


#endif // SH_MATERIAL_INFO_H