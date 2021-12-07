#include "shVkMemoryInfo.h"
#include "shVkCore.h"
#include "shUtilities.h"

#include "shMesh.h"
#include "shMeshInfo.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <shVkPipelineData.h>

void shCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlagBits usage_flags, VkBuffer* buffer) {
	VkBufferCreateInfo buffer_create_info = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		(VkDeviceSize)size,						//size;
		usage_flags,							//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};

#ifndef NDEBUG
	printf("creating buffer of %u bytes \n", size);
#endif	

	shCheckVkResult(
		vkCreateBuffer(device, &buffer_create_info, NULL, buffer),
		"error creating buffer"
	);
}

void shCreateVertexBuffer(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh) {
	shCreateBuffer(core.device, mesh_info.vertex_count * sizeof(mesh_info.p_vertices[0]), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &mesh->vertex_buffer);
}

void shAllocateVertexBuffer(const ShVkCore core, ShMesh* mesh) {
	shAllocateMemory(core.device, core.physical_device, mesh->vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mesh->vertex_buffer_memory);
}

void shMapVertexBufferMemory(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh) {
	shMapMemory(core.device, mesh->vertex_buffer_memory, mesh_info.vertex_count * sizeof(mesh_info.p_vertices[0]), (void*)mesh_info.p_vertices);
}

void shCreateIndexBuffer(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh) {
	shCreateBuffer(core.device, mesh_info.index_count * sizeof(mesh_info.p_indices[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &mesh->index_buffer);
}

void shAllocateIndexBuffer(const ShVkCore core, ShMesh* mesh) {
	shAllocateMemory(core.device, core.physical_device, mesh->index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &mesh->index_buffer_memory);
}

void shMapIndexBufferMemory(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh) {
	shMapMemory(core.device, mesh->index_buffer_memory, mesh_info.index_count * sizeof(mesh_info.p_indices[0]), (void*)mesh_info.p_indices);
}

void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory) {

	uint32_t memoryTypeIndex = 0;
	shGetMemoryType(device, physical_device, buffer, typeFlags, &memoryTypeIndex);

	VkMemoryRequirements memoryRequirements;
	vkGetBufferMemoryRequirements(device, buffer, &memoryRequirements);

	VkMemoryAllocateInfo memoryAllocateInfo = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,	//sType;
		NULL,									//pNext;
		memoryRequirements.size,				//allocationSize;
		memoryTypeIndex							//memoryTypeIndex;
	};

#ifndef NDEBUG
	printf("allocating %u bytes of memory\n", (uint32_t)memoryRequirements.size);
#endif

	shCheckVkResult(
		vkAllocateMemory(device, &memoryAllocateInfo, NULL, pMemory),
		"error allocating memory"
	);

#ifndef NDEBUG
	printf("binding memory\n");
#endif

	shCheckVkResult(
		vkBindBufferMemory(device, buffer, *pMemory, 0),
		"error binding buffer memory"
	);
}

void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex) {
	
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physical_device, &memoryProperties);

	for (uint32_t i = 0; i < memoryProperties.memoryTypeCount; i++) {
		
		if (memoryProperties.memoryTypes[i].propertyFlags & (1 << i) && 
			(memoryProperties.memoryTypes[i].propertyFlags & typeFlags) == typeFlags) {
			*memoryTypeIndex = i;
			return;
		}

	}

#ifndef NDEBUG
	printf("cannot find suitable memory type for given buffer \n");
#endif 
	exit(-1);
}

void shMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void *pData) {
	
#ifndef NDEBUG
	//printf("mapping %u bytes of memory \n", dataSize);
#endif 
	
	void* data;
	shCheckVkResult(
		vkMapMemory(device, memory, 0, dataSize, 0, &data),
		"error mapping memory"
	);
	memcpy(data, pData, (size_t)dataSize);
	vkUnmapMemory(device, memory);
}

void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory) {
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);
}