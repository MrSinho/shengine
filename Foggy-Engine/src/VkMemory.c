#include "VkMemory.h"
#include "Utilities.h"
#include "VkDataHandler.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CreateVertexBuffer(const VkDevice device, VkBuffer* vertexBuffer, const uint32_t bufferSize) {
	VkBufferCreateInfo vertexBufferCreateInfo = {
		VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,	//sType;
		NULL,									//pNext;
		0,										//flags;
		bufferSize,								//size;
		VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,		//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};

#ifndef NDEBUG
	printf("creating vertex buffer of %u bytes \n", bufferSize);
#endif	

	CheckVkResult(
		vkCreateBuffer(device, &vertexBufferCreateInfo, NULL, vertexBuffer),
		"error creating vertex buffer"
	);
}

void LoadMesh(const VkData data, const Mesh mesh, VkBuffer *vertexBuffer) {

	CreateVertexBuffer(data.device, vertexBuffer, mesh.vertexCount * sizeof(mesh.pVertices[0]));

	VkDeviceMemory vertexBufferMemory;
	AllocateMemory(data.device, data.physicalDevice, *vertexBuffer, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, &vertexBufferMemory);
		
	MapMemory(data.device, vertexBufferMemory, mesh.vertexCount * sizeof(mesh.pVertices[0]), (void*)mesh.pVertices);
}

void AllocateMemory(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory) {

	uint32_t memoryTypeIndex = 0;
	GetMemoryType(device, physicalDevice, buffer, typeFlags, &memoryTypeIndex);

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

	CheckVkResult(
		vkAllocateMemory(device, &memoryAllocateInfo, NULL, pMemory),
		"error allocating memory"
	);

#ifndef NDEBUG
	printf("binding memory\n");
#endif

	CheckVkResult(
		vkBindBufferMemory(device, buffer, *pMemory, 0),
		"error binding buffer memory"
	);
}

void GetMemoryType(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex) {
	
	VkPhysicalDeviceMemoryProperties memoryProperties;
	vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memoryProperties);

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

void MapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void *pData) {
	
#ifndef NDEBUG
	printf("mapping %u bytes of memory \n", dataSize);
#endif 
	
	void* data;
	CheckVkResult(
		vkMapMemory(device, memory, 0, dataSize, 0, &data),
		"error mapping memory"
	);
	memcpy(data, pData, (size_t)dataSize);
	vkUnmapMemory(device, memory);
}

void ClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory) {
	vkDestroyBuffer(device, buffer, NULL);
	vkFreeMemory(device, memory, NULL);
}