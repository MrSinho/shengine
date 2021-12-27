#include "shVkMemoryInfo.h"
#include "shVkCore.h"
#include "shUtilities.h"

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

void shAllocateMemory(const VkDevice device, const VkPhysicalDevice physical_device, const VkBuffer buffer, const uint32_t typeFlags, VkDeviceMemory *pMemory) {

	uint32_t memoryTypeIndex = 0;
	shGetMemoryType(device, physical_device, typeFlags, &memoryTypeIndex);

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

void shGetMemoryType(const VkDevice device, const VkPhysicalDevice physical_device, const uint32_t typeFlags, uint32_t *memoryTypeIndex) {
	
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

void shCreateImage(ShVkCore core, const uint32_t width, const uint32_t height, VkFormat format, VkImageUsageFlags usage, VkImage* p_image, VkDeviceMemory* p_image_memory) {
	VkExtent3D image_extent = {
		width, height, 1
	};

	VkImageCreateInfo image_create_info = {
		VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,	//sType;			
		NULL,									//pNext;
		0,										//flags;
		VK_IMAGE_TYPE_2D,						//imageType;
		format,									//format;
		image_extent,							//extent;
		1,										//mipLevels;
		1,										//arrayLayers;
		VK_SAMPLE_COUNT_1_BIT,					//samples;
		VK_IMAGE_TILING_OPTIMAL,				//tiling;
		usage,									//usage;
		VK_SHARING_MODE_EXCLUSIVE,				//sharingMode;
		0,										//queueFamilyIndexCount;
		NULL									//pQueueFamilyIndices;
	};
	shCheckVkResult(vkCreateImage(core.device, &image_create_info, NULL, p_image),
		"error creating image"
	);

	VkMemoryRequirements memory_requirements;
	vkGetImageMemoryRequirements(core.device, *p_image, &memory_requirements);

	uint32_t memory_type_index = 0;
	shGetMemoryType(core.device, core.physical_device, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &memory_type_index);

	VkMemoryAllocateInfo memory_allocate_info = {
		VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		NULL,
		memory_requirements.size,
		memory_type_index
	};

	shCheckVkResult(vkAllocateMemory(core.device, &memory_allocate_info, NULL, p_image_memory),
		"error allocating image memory"
	);

	vkBindImageMemory(core.device, *p_image, *p_image_memory, 0);
}