#ifndef SH_MEMORY_INFO_H
#define SH_MEMORY_INFO_H

#include <stdint.h>

#include <vulkan/vulkan.h>

typedef struct ShVkCore ShVkCore;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShMesh ShMesh;



extern void shCreateBuffer(const VkDevice device, const uint32_t bufferSize, VkBufferUsageFlagBits usage_flags, VkBuffer* buffer);

extern void shCreateVertexBuffer(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh);

void shAllocateVertexBuffer(const ShVkCore core, ShMesh* mesh);

extern void shMapVertexBufferMemory(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh);

extern void shCreateIndexBuffer(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh);

void shAllocateIndexBuffer(const ShVkCore core, ShMesh* mesh);

extern void shMapIndexBufferMemory(const ShVkCore core, const ShMeshInfo mesh_info, ShMesh* mesh);

extern void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, uint32_t *memoryTypeIndex);

extern void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physicalDevice, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory);

extern void shMapMemory(const VkDevice device, const VkDeviceMemory memory, const uint32_t dataSize, const void* pData);



extern void shClearBufferMemory(const VkDevice device, const VkBuffer buffer, const VkDeviceMemory memory);

#endif
