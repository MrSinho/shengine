#ifndef SH_MESH_INFO_H
#define SH_MESH_INFO_H


#include <stdint.h>
#include <vulkan/vulkan.h>

#include "shEcsImplementation.h"

#ifdef _MSC_VER
#pragma warning ( disable : 6386)
#endif // _MSC_VER

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

static void shGenerateGraphIndices(ShMeshInfo* mesh) {
	mesh->index_count = mesh->vertex_count / 3 * 2 - 2;
	if (mesh->index_count == 0) { return; }
	mesh->p_indices = calloc(mesh->index_count, sizeof(uint32_t));
	if (mesh->p_indices == NULL) { return; }
	mesh->p_indices[0] = 0;
	mesh->p_indices[mesh->index_count - 1] = mesh->vertex_count / 3 - 1;
	uint32_t vertex = 1;
	for (uint32_t i = 1; i < mesh->index_count - 1; i += 2) {
		mesh->p_indices[i] = vertex;
		mesh->p_indices[i + 1] = vertex;
		vertex++;
	}
}


SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMeshInfo, 1)

#endif // SH_MESH_INFO_H