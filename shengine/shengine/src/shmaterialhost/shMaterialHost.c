#ifdef __cplusplus
extern "C" {
#endif

#include "shecs/shMaterial.h"
#include "shengine/shEngine.h"

#include "shvulkan/shVkDrawloop.h"

uint32_t shGetUniformOffset(ShMaterialHost* p_material, const uint32_t uniform_idx) {
	uint32_t uniform_offset = 0;
	for (uint32_t i = 0; i < p_material->pipeline.uniform_count; i++) {
		if (i == uniform_idx) {
			return uniform_offset;
		}
		uniform_offset += p_material->pipeline.uniform_buffers_size[i];
	}
	return uniform_offset;
}

uint32_t shGetUniformTotalSize(ShMaterialHost* p_material) {
	uint32_t uniform_total_size = 0;
	for (uint32_t i = 0; i < p_material->pipeline.uniform_count; i++) {
		uniform_total_size += p_material->pipeline.uniform_buffers_size[i];
	}
	return uniform_total_size;
}

uint8_t shEntityInMaterial(const uint32_t entity, ShMaterialHost* p_material) {
	for (uint32_t i = 0; i < p_material->entity_count; i++) {
		if (entity == p_material->entities[i]) { return 1; }
	}
	return 0;
}

void shUpdateUniformParameters(ShEngine* p_engine, const uint32_t uniform_idx, ShMaterialHost* p_material) {
	vkDeviceWaitIdle(p_engine->core.device);
	uint32_t uniform_offset = shGetUniformOffset(p_material, uniform_idx);
	shWriteUniformBufferMemory(&p_engine->core, uniform_idx, &((char*)p_material->uniform_buffers)[uniform_offset], &p_material->pipeline);
	shBindUniformBuffer(&p_engine->core, uniform_idx, &p_material->pipeline);
}

#ifdef __cplusplus
}
#endif