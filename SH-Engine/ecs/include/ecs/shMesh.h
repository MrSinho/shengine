#ifndef SH_MESH_H
#define SH_MESH_H

#include <shVkMemoryInfo.h>

#include "shEcsImplementation.h"

typedef enum ShMeshInfoFlags {
	SH_MESH_SETUP_STATIC_MESH  = 0b001,
	SH_MESH_SETUP_DYNAMIC_MESH = 0b010,
	SH_MESH_SETUP_RUNTIME_MESH = 0b100,
} ShMeshInfoFlags;

typedef struct ShMeshInfo {
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
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMesh, 4)

#endif//SH_MESH_H