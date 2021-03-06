#ifndef SH_MESH_H
#define SH_MESH_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkMemoryInfo.h>

#include "shEcsImplementation.h"

typedef enum ShMeshInfoFlags {
	SH_MESH_SETUP_STATIC_MESH  = 0b001,
	SH_MESH_SETUP_DYNAMIC_MESH = 0b010,
} ShMeshInfoFlags;

typedef struct ShMeshInfo {
	uint32_t		vertex_stride;
	uint32_t		vertex_count;
	float*			p_vertices;
	uint32_t		index_count;
	uint32_t*		p_indices;
	ShMeshInfoFlags flags;
} ShMeshInfo;

typedef struct ShMesh {
	ShMeshInfo		mesh_info;
	VkBuffer 		vertex_buffer;
	VkDeviceMemory 	vertex_buffer_memory;
	VkBuffer		index_buffer;
	VkDeviceMemory	index_buffer_memory;
} ShMesh;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMesh, 1)

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_MESH_H