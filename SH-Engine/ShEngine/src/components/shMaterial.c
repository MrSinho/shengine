#include "ecs/shMaterial.h" 

#include <memory.h>
#include <shVkCore.h>

#include "shfd/shFile.h"

void shSetupShaders(const ShVkCore core, const char* vertexspv, const char* fragmentspv, ShVkPipelineData* p_pipe_data) {

	p_pipe_data->shader_module_count = 2;
	p_pipe_data->shaderStageCount = 2;

	uint32_t vertex_size, fragment_size = 0;
	const char* vertex_code		= shReadBinary(vertexspv, &vertex_size);
	const char* fragment_code	= shReadBinary(fragmentspv, &fragment_size);

	p_pipe_data->p_shader_modules = calloc(p_pipe_data->shader_module_count, sizeof(VkShaderModule));
	if (p_pipe_data->p_shader_modules != NULL) {
		shCreateShaderModule(core.device, vertex_size, vertex_code, &p_pipe_data->p_shader_modules[0]);
		shCreateShaderModule(core.device, fragment_size, fragment_code, &p_pipe_data->p_shader_modules[1]);

		p_pipe_data->p_shader_stages = calloc(p_pipe_data->shader_module_count, sizeof(VkPipelineShaderStageCreateInfo));
		if (p_pipe_data->p_shader_stages != NULL) {
			shCreateShaderStage(core.device, p_pipe_data->p_shader_modules[0], VK_SHADER_STAGE_VERTEX_BIT, &p_pipe_data->p_shader_stages[0]);
			shCreateShaderStage(core.device, p_pipe_data->p_shader_modules[1], VK_SHADER_STAGE_FRAGMENT_BIT, &p_pipe_data->p_shader_stages[1]);
		}
	}

}

void shSetupMaterial(const ShVkCore core, ShMaterial* p_material) {
	assert(p_material != NULL);
	ShMaterial mat = {
		0,				//fixed_states
		0				//pipeline_data;
	};

	shSetFixedStates(core, p_material->material_info.fixed_states_flags, &mat.fixed_states);

	mat.pipeline_data.uniform_buffer_count = p_material->material_info.uniform_buffer_count;
	mat.pipeline_data.p_uniform_buffers			= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(ShUniformBuffer));
	mat.pipeline_data.p_descriptor_sets			= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(VkDescriptorSet));
	mat.pipeline_data.p_write_descriptor_sets	= calloc(mat.pipeline_data.uniform_buffer_count, sizeof(VkWriteDescriptorSet));
	for (uint32_t i = 0; i < p_material->material_info.uniform_buffer_count; i++) {
		if (p_material->material_info.p_uniform_buffers[i].uniformSize != 0 && p_material->material_info.p_uniform_buffers[i].uniformStage != 0) {
			shAllocateUniformBufferData(core, p_material->material_info.p_uniform_buffers[i].uniformSize, &mat.pipeline_data.p_uniform_buffers[i]);
			shCreateDescriptorPool(core, &mat.pipeline_data.p_uniform_buffers[i]);
			shDescriptorSetLayout(core, 0, p_material->material_info.p_uniform_buffers[i].uniformStage, &mat.pipeline_data.p_uniform_buffers[i]);
			shAllocateDescriptorSets(core, i, &mat.pipeline_data);/*work here*/
		}
	}

	if (p_material->material_info.pConstSize != 0 && p_material->material_info.pConstStage != 0) {
		shSetPushConstants(p_material->material_info.pConstStage, 0, p_material->material_info.pConstSize, &mat.pipeline_data);
	}

	shSetupShaders(core, p_material->material_info.vertex_shader_path, p_material->material_info.fragment_shader_path, &mat.pipeline_data);

	shSetupGraphicsPipeline(core, mat.fixed_states, &mat.pipeline_data);

	*p_material = mat;
}