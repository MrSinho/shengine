#ifndef FGG_MESH_H
#define FGG_MESH_H

#include "fggEcsImplementation.h"

#include <stdint.h>
#include <vulkan/vulkan.h>

typedef struct FggMesh {

	uint32_t		vertexCount;
	float*			pVertices;
	uint32_t		indexCount;
	uint32_t*		pIndices;
	VkBuffer 		vertexBuffer;
	VkDeviceMemory 	vertexBufferMemory;

} FggMesh;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 1)

#endif