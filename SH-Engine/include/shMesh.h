#ifndef SH_MESH_H
#define SH_MESH_H

#include <vulkan/vulkan.h>

#include <shEcsImplementation.h>

typedef struct ShMesh {

	VkBuffer 		vertex_buffer;
	VkDeviceMemory 	vertex_buffer_memory;

	VkBuffer		index_buffer;
	VkDeviceMemory	index_buffer_memory;

} ShMesh;

SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMesh, 4)

#endif // SH_MESH_H