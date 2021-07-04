#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

#include <vulkan/vulkan.h>

typedef struct Mesh {

	uint32_t		vertexCount;
	float*			pVertices;
	uint32_t		indexCount;
	uint32_t*		pIndices;
	VkBuffer 		vertexBuffer;
	VkDeviceMemory 	vertexBufferMemory;

} Mesh;

typedef struct VkData VkData;

extern void CreateVertexBuffer(const VkDevice device, VkBuffer* vertexBuffer, const uint32_t bufferSize);
extern void LoadMesh(const VkData data, Mesh *mesh);

extern void GetMemoryType(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex);
extern void AllocateMemory(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory);
extern void MapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void* pData);

extern void ClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

#endif
