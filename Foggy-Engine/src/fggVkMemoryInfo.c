#include "fggVkMemoryInfo.h"
#include "fggVkCore.h"
#include "fggUtilities.h"

#include "fggMesh.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fggVkPipelineData.h>

void fggCreateBuffer(const VkDevice device, const uint32_t size, VkBufferUsageFlagBits bufferUsage, VkBuffer* buffer) {
	VkBufferCreateInfo bufferCreateInfo = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		(VkDeviceSize)size,						//size;
		bufferUsage,							//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};

#ifndef NDEBUG
	printf("creating buffer of %u bytes \n", size);
#endif	

	fggCheckVkResult(
		vkCreateBuffer(device, &bufferCreateInfo, NULL, buffer),
		"error creating buffer"
	);
}

void fggAllocateMeshVertexData(const FggVkCore core, FggMesh *mesh) {
	fggCreateBuffer(core.device, mesh->vertex_count * sizeof(mesh->p_vertices[0]), VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, &mesh->vertex_buffer);
	fggAllocateMemory(core.device, core.physical_device, mesh->vertex_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &mesh->vertex_buffer_memory);
}

void fggMapVertexBufferMemory(const FggVkCore core, FggMesh* mesh) {
	fggMapMemory(core.device, mesh->vertex_buffer_memory, mesh->vertex_count * sizeof(mesh->p_vertices[0]), (void*)mesh->p_vertices);
}

void fggMapIndexBufferMemory(const FggVkCore core, FggMesh* mesh) {
	fggMapMemory(core.device, mesh->vertex_buffer_memory, mesh->vertex_count * sizeof(mesh->p_vertices[0]), (void*)mesh->p_vertices);
}

void fggAllocateMeshIndexData(const FggVkCore core, FggMesh* mesh) {
	fggCreateBuffer(core.device, mesh->index_count * sizeof(mesh->p_indices[0]), VK_BUFFER_USAGE_INDEX_BUFFER_BIT, &mesh->index_buffer);
	fggAllocateMemory(core.device, core.physical_device, mesh->index_buffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &mesh->index_buffer_memory);
	fggMapMemory(core.device, mesh->index_buffer_memory, mesh->index_count * sizeof(mesh->p_indices[0]), (void*)mesh->p_indices);
}

void fggAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory) {

	uint32_t memoryTypeIndex = 0;
	fggGetMemoryType(device, physical_device, buffer, typeFlags, &memoryTypeIndex);

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

	fggCheckVkResult(
		vkAllocateMemory(device, &memoryAllocateInfo, NULL, pMemory),
		"error allocating memory"
	);

#ifndef NDEBUG
	printf("binding memory\n");
#endif

	fggCheckVkResult(
		vkBindBufferMemory(device, buffer, *pMemory, 0),
		"error binding buffer memory"
	);
}

void fggGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex) {
	
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

void fggMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void *pData) {
	
#ifndef NDEBUG
	//printf("mapping %u bytes of memory \n", dataSize);
#endif 
	
	void* data;
	fggCheckVkResult(
		vkMapMemory(device, memory, 0, dataSize, 0, &data),
		"error mapping memory"
	);
	memcpy(data, pData, (size_t)dataSize);
	vkUnmapMemory(device, memory);
}

void fggClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory) {
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);
}