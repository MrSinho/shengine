#include "shMaterial.h" 

#include <memory.h>

void shSetupMaterial(const ShVkCore core, const ShMaterialInfo mat_info, ShMaterial* p_material) {

	ShMaterial mat = {
		mat_info,
		0,				//fixed_states
		0				//pipeline_data;
	};

	shSetFixedStates(core, mat_info.fixed_states_flags, &mat.fixed_states);

	ShPipelineSetupFlags pipeFlags = 0;

	if (mat_info.uniformSize != 0 && mat_info.uniformStage != 0) {
		shAllocateUniformBufferData(core, mat_info.uniformSize, &mat.pipeline_data);
		shDescriptorSetLayout(core, 0, mat_info.uniformStage, &mat.pipeline_data);
		shCreateDescriptorPool(core, &mat.pipeline_data);
		shAllocateDescriptorSets(core, &mat.pipeline_data);
		pipeFlags |= SH_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	}
	
	if (mat_info.pConstSize != 0 && mat_info.pConstStage != 0) {
		shSetPushConstants(mat_info.pConstStage, 0, mat_info.pConstSize, &mat.pipeline_data);
		pipeFlags |= SH_PIPELINE_SETUP_PUSH_CONSTANTS_BIT;
	}

	shSetupShaders(core, mat_info.vertex_shader_path, mat_info.fragment_shader_path, &mat.pipeline_data);

	shSetupGraphicsPipeline(core, mat.fixed_states, pipeFlags, &mat.pipeline_data);

	*p_material = mat;
}

void shCreateMaterialInstance(const ShVkCore core, ShMaterial* src, ShMaterial* dst) {
	shSetupMaterial(core, src->mat_info, dst);
}