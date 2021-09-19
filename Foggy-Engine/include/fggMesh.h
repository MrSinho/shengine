#ifndef FGG_MESH_H
#define FGG_MESH_H


#include <stdint.h>
#include <vulkan/vulkan.h>

#include "fggEcsImplementation.h"

typedef enum FggMeshSetupFlags {
	FGG_MESH_SETUP_STATIC_MESH  = 0b001,
	FGG_MESH_SETUP_DYNAMIC_MESH = 0b010,
	FGG_MESH_SETUP_RUNTIME_MESH = 0b100,
} FggMeshSetupFlags;

typedef struct FggMesh {

	uint32_t		vertex_count;
	float*			p_vertices;

	uint32_t		index_count;
	uint32_t*		p_indices;

	FggMeshSetupFlags flags;

	VkBuffer 		vertex_buffer;
	VkDeviceMemory 	vertex_buffer_memory;

	VkBuffer		index_buffer;
	VkDeviceMemory	index_buffer_memory;

} FggMesh;

static void fggGenerateGraphIndices(FggMesh* mesh) {
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


EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 1)

#endif