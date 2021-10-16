#include "fggMaterial.h" 

#include <memory.h>

void fggSetupMaterial(const FggVkCore core, const FggMaterialInfo mat_info, FggMaterial* p_material) {

	FggMaterial mat = {
		mat_info,
		0,				//fixed_states
		0				//pipeline_data;
	};

	fggSetFixedStates(core, mat_info.fixed_states_flags, &mat.fixed_states);

	FggPipelineSetupFlags pipeFlags = 0;

	if (mat_info.uniformSize != 0 && mat_info.uniformStage != 0) {
		fggAllocateUniformBufferData(core, mat_info.uniformSize, &mat.pipeline_data);
		fggDescriptorSetLayout(core, 0, mat_info.uniformStage, &mat.pipeline_data);
		fggCreateDescriptorPool(core, &mat.pipeline_data);
		fggAllocateDescriptorSets(core, &mat.pipeline_data);
		pipeFlags |= FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	}
	
	if (mat_info.pConstSize != 0 && mat_info.pConstStage != 0) {
		fggSetPushConstants(mat_info.pConstStage, 0, mat_info.pConstSize, &mat.pipeline_data);
		pipeFlags |= FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT;
	}

	fggSetupShaders(core, mat_info.vertex_shader_path, mat_info.fragment_shader_path, &mat.pipeline_data);

	fggSetupGraphicsPipeline(core, mat.fixed_states, pipeFlags, &mat.pipeline_data);

	*p_material = mat;
}

void fggCreateMaterialInstance(const FggVkCore core, FggMaterial* src, FggMaterial* dst) {
	fggSetupMaterial(core, src->mat_info, dst);
	//not really a material instance
	
	//memcpy(dst, src, sizeof(FggMaterial));
	//if (src->mat_info.uniformSize != 0 && src->mat_info.uniformStage != 0) {
	//	fggAllocateUniformBufferData(core, src->mat_info.uniformSize, &dst->pipeline_data);
	//	fggDescriptorSetLayout(core, 0, src->mat_info.uniformStage, &dst->pipeline_data);
	//	fggCreateDescriptorPool(core, &dst->pipeline_data);
	//	fggAllocateDescriptorSets(core, &dst->pipeline_data);
	//}
	//fggSetupGraphicsPipeline(core, src->fixed_states, src->pipeline_data.setupFlags, &dst->pipeline_data);

	//dst->pipeline_data.shared_uniform = 1;
	//dst->p_main_material = src;
	//src->material_instance_count++;
	//
	//FggMaterial* p_material_instances = (FggMaterial*)calloc(src->material_instance_count, sizeof(FggMaterial));
	//if (p_material_instances == NULL) { return; }
	//memcpy((void*)p_material_instances, src->p_material_instances, src->material_instance_count-1);
	//p_material_instances[src->material_instance_count - 1] = *dst;
}