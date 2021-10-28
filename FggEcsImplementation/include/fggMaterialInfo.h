#ifndef FGG_MATERIAL_INFO_H
#define FGG_MATERIAL_INFO_H

#include "fggEcsImplementation.h"

typedef enum FggFixedStateFlags {
	FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT = 0x01,
	FGG_FIXED_STATES_POLYGON_MODE_FACE_BIT = 0x02,
	FGG_FIXED_STATES_POLYGON_MODE_POINTS_BIT = 0x04,
	FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST = 0x08,
	FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST = 0x10,
	FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST = 0x20,
	FGG_FIXED_STATES_VERTEX_POSITIONS_BIT = 0x40,
	FGG_FIXED_STATES_VERTEX_NORMALS_BIT = 0x80,
	FGG_FIXED_STATES_VERTEX_TCOORDS_BIT = 0x100
} FggFixedStateFlags;



typedef enum FggShaderStageFlags {

	FGG_SHADER_STAGE_VERTEX_BIT 	= 0x00000001,
    FGG_SHADER_STAGE_GEOMETRY_BIT 	= 0x00000008,
    FGG_SHADER_STAGE_FRAGMENT_BIT 	= 0x00000010

} FggShaderStageFlags;



typedef struct FggMaterialInfo {
	const char*				vertex_shader_src_path;
	const char*				fragment_shader_src_path;
	const char* 			vertex_shader_path;
	const char* 			fragment_shader_path;	
	uint32_t				uniformSize;
	FggShaderStageFlags		uniformStage;
	uint32_t				pConstSize; 
	FggShaderStageFlags		pConstStage;
	FggFixedStateFlags		fixed_states_flags;

} FggMaterialInfo;

FGG_ECS_MAKE_COMPONENT_DEFINITIONS(FggMaterialInfo, 5)

#endif // FGG_MATERIAL_INFO_H