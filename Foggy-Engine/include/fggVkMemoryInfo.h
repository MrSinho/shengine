#ifndef FGG_MEMORY_INFO_H
#define FGG_MEMORY_INFO_H

#include <stdint.h>

#include <vulkan/vulkan.h>

typedef struct FggVkCore FggVkCore;

typedef struct FggMeshInfo FggMeshInfo;

typedef struct FggMesh FggMesh;



extern void fggCreateBuffer(const VkDevice device, const uint32_t bufferSize, VkBufferUsageFlagBits usage_flags, VkBuffer* buffer);

extern void fggCreateVertexBuffer(const FggVkCore core, const FggMeshInfo mesh_info, FggMesh* mesh);

void fggAllocateVertexBuffer(const FggVkCore core, FggMesh* mesh);

extern void fggMapVertexBufferMemory(const FggVkCore core, const FggMeshInfo mesh_info, FggMesh* mesh);

extern void fggCreateIndexBuffer(const FggVkCore core, const FggMeshInfo mesh_info, FggMesh* mesh);

void fggAllocateIndexBuffer(const FggVkCore core, FggMesh* mesh);

extern void fggMapIndexBufferMemory(const FggVkCore core, const FggMeshInfo mesh_info, FggMesh* mesh);

extern void fggGetMemoryType(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex);

extern void fggAllocateMemory(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory);

extern void fggMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void* pData);



extern void fggClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

#endif
