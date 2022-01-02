#include "shMaterial.h" 

#include <memory.h>

void shSetupMaterial(const ShVkCore core, const ShMaterialInfo mat_info, ShMaterial* p_material) {

	ShMaterial mat = {
		0,				//fixed_states
		0				//pipeline_data;
	};

	shSetFixedStates(core, mat_info.fixed_states_flags, &mat.fixed_states);

	ShPipelineSetupFlags pipeFlags = 0;

	mat.pipeline_data.uniform_buffer_count = mat_info.uniform_buffer_count;
	mat.pipeline_data.p_uniform_buffers			= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(ShUniformBuffer));
	mat.pipeline_data.p_descriptor_sets			= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(VkDescriptorSet));
	mat.pipeline_data.p_write_descriptor_sets	= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(VkWriteDescriptorSet));
	for (uint32_t i = 0; i < mat_info.uniform_buffer_count; i++) {
		if (mat_info.p_uniform_buffers[i].uniformSize != 0 && mat_info.p_uniform_buffers[i].uniformStage != 0) {
			shAllocateUniformBufferData(core, mat_info.p_uniform_buffers[i].uniformSize, &mat.pipeline_data.p_uniform_buffers[i]);
			shCreateDescriptorPool(core, &mat.pipeline_data.p_uniform_buffers[i]);
			shDescriptorSetLayout(core, 0, mat_info.p_uniform_buffers[i].uniformStage, &mat.pipeline_data.p_uniform_buffers[i]);
			shAllocateDescriptorSets(core, i, &mat.pipeline_data);/*work here*/
		}
	}
	
	if (mat_info.pConstSize != 0 && mat_info.pConstStage != 0) {
		shSetPushConstants(mat_info.pConstStage, 0, mat_info.pConstSize, &mat.pipeline_data);
	}

	shSetupShaders(core, mat_info.vertex_shader_path, mat_info.fragment_shader_path, &mat.pipeline_data);

	shSetupGraphicsPipeline(core, mat.fixed_states, pipeFlags, &mat.pipeline_data);

	*p_material = mat;
}