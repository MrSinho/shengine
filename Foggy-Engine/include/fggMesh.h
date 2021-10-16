#ifndef FGG_MESH_H
#define FGG_MESH_H

#include <vulkan/vulkan.h>

#include <fggEcsImplementation.h>

typedef struct FggMesh {

	VkBuffer 		vertex_buffer;
	VkDeviceMemory 	vertex_buffer_memory;

	VkBuffer		index_buffer;
	VkDeviceMemory	index_buffer_memory;

} FggMesh;

FGG_ECS_MAKE_COMPONENT_DEFINITIONS(FggMesh, 4)

#endif // FGG_MESH_H