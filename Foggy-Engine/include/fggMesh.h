#ifndef FGG_MESH_H
#define FGG_MESH_H


#include <stdint.h>
#include <vulkan/vulkan.h>

#include "fggEcsImplementation.h"

typedef enum FggMeshSetupFlags {
	FGG_MESH_SETUP_STATIC_MESH  = 0b001,
	FGG_MESH_SETUP_DYNAMIC_MESH = 0b010
} FggMeshSetupFlags;

typedef struct FggMesh {

	uint32_t		vertexCount;
	float*			pVertices;

	uint32_t		indexCount;
	uint32_t*		pIndices;

	VkBuffer 		vertexBuffer;
	VkDeviceMemory 	vertexBufferMemory;

	VkBuffer		indexBuffer;
	VkDeviceMemory	indexBufferMemory;

	FggMeshSetupFlags flags;

} FggMesh;

static void fggGenerateGraphIndices(FggMesh* mesh) {
	mesh->indexCount = mesh->vertexCount / 3 * 2 - 2;
	mesh->pIndices = calloc(mesh->indexCount, sizeof(uint32_t));
	if (mesh->pIndices == NULL) { return; }
	mesh->pIndices[0] = 0;
	mesh->pIndices[mesh->indexCount - 1] = mesh->vertexCount / 3 - 1;
	uint32_t vertex = 1;
	for (uint32_t i = 1; i < mesh->indexCount - 1; i += 2) {
		mesh->pIndices[i] = vertex;
		mesh->pIndices[i + 1] = vertex;
		vertex++;
	}
}


EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 1)

#endif