#ifndef FGG_MESH_H
#define FGG_MESH_H


#include <stdint.h>
#include <vulkan/vulkan.h>


#include "fggEcsImplementation.h"
#include "fggExport.h"


typedef struct FggMesh {

	uint32_t		vertexCount;
	float*			pVertices;

	uint32_t		indexCount;
	uint32_t*		pIndices;

	VkBuffer 		vertexBuffer;
	VkDeviceMemory 	vertexBufferMemory;

	VkBuffer		indexBuffer;
	VkDeviceMemory	indexBufferMemory;

} FggMesh;



EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 1)

FGG_DEFINE_EXPORT_OFFSET(FggMesh, 0);

FGG_DEFINE_EXPORT_SIZE(FggMesh, sizeof(FggMesh) - sizeof(VkBuffer) * 2 - sizeof(VkDeviceMemory) * 2);

#endif