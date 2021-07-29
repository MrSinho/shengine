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



EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 1)

#endif