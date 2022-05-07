#ifdef __cplusplus
extern "C" {
#endif

#include "shecs/shMaterial.h"
#include "shengine/shEngine.h"

#include "shvulkan/shVkDrawLoop.h"
#include "shvulkan/shVkMemoryInfo.h"

uint32_t shGetUniformOffset(ShMaterialHost* p_material, const uint32_t descriptor_idx) {
	uint32_t descriptor_offset = 0;
	for (uint32_t i = 0; i < p_material->pipeline.descriptor_count; i++) {
		if (i == descriptor_idx) {
			return descriptor_offset;
		}
		descriptor_offset += p_material->pipeline.descriptor_buffer_infos[i].range;
	}
	return descriptor_offset;
}

uint32_t shGetUniformTotalSize(ShMaterialHost* p_material) {
	uint32_t descriptor_total_size = 0;
	for (uint32_t i = 0; i < p_material->pipeline.descriptor_count; i++) {
		descriptor_total_size += p_material->pipeline.descriptor_buffer_infos[i].range;
	}
	return descriptor_total_size;
}

uint8_t shEntityInMaterial(const uint32_t entity, ShMaterialHost* p_material) {
	for (uint32_t i = 0; i < p_material->entity_count; i++) {
		if (entity == p_material->entities[i]) { return 1; }
	}
	return 0;
}

void shUpdateUniformParameters(ShEngine* p_engine, const uint32_t descriptor_idx, ShMaterialHost* p_material) {
	vkDeviceWaitIdle(p_engine->core.device);
	uint32_t descriptor_offset = shGetUniformOffset(p_material, descriptor_idx);
	shPipelineWriteDescriptorBufferMemory(&p_engine->core.device, descriptor_idx, &((char*)p_material->uniform_buffers)[descriptor_offset], &p_material->pipeline);
	shPipelineBindDescriptorSet(&p_engine->core.graphics_cmd_buffer, descriptor_idx, VK_PIPELINE_BIND_POINT_GRAPHICS, &p_material->pipeline);
}

#ifdef __cplusplus
}
#endif
