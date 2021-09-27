#include "fggMaterial.h" 

#include <memory.h>

void fggSetupMaterial(const FggVkCore core, const char* vPath, const char* fPath, const uint32_t uniformSize, const VkShaderStageFlags uniformStage, const uint32_t pConstSize, const VkShaderStageFlags pConstStage, const FggVkFixedStates fixed_states, FggMaterial* pMaterial) {
	FggMaterial mat = {
		vPath,			//vertex_shader_path;
		fPath,			//fragment_shader_path;	
		fixed_states,	//pFixedStates
		0,				//pipeline_data;
	};

	FggPipelineSetupFlags pipeFlags = 0;

	if (uniformSize != 0 && uniformStage != 0) {	
		fggAllocateUniformBufferData(core, uniformSize, &mat.pipeline_data);
		fggDescriptorSetLayout(core, 0, uniformStage, &mat.pipeline_data);
		fggCreateDescriptorPool(core, &mat.pipeline_data);
		fggAllocateDescriptorSets(core, &mat.pipeline_data);
		pipeFlags |= FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	}
	
	if (pConstSize != 0 && pConstStage != 0) {
		fggSetPushConstants(pConstStage, 0, pConstSize, &mat.pipeline_data);
		pipeFlags |= FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT;
	}

	fggSetupShaders(core, mat.vertex_shader_path, mat.fragment_shader_path, &mat.pipeline_data);

	fggSetupGraphicsPipeline(core, fixed_states, pipeFlags, &mat.pipeline_data);

	*pMaterial = mat;
}

void fggCreateMaterialInstance(const FggVkCore core, FggMaterial* src, FggMaterial* dst) {
	fggSetupMaterial(core,
		src->vertex_shader_path, src->fragment_shader_path,
		src->pipeline_data.uniformBufferSize, src->pipeline_data.descriptorSetLayoutBinding.stageFlags,
		src->pipeline_data.pushConstantRange.size, src->pipeline_data.pushConstantRange.stageFlags,
		src->fixed_states, dst);
	//memcpy(dst, src, sizeof(FggMaterial));
	//dst->pipeline_data.shared_uniform = 1;
	//dst->p_main_material = src;
	//src->material_instance_count++;
	//
	//FggMaterial* p_material_instances = (FggMaterial*)calloc(src->material_instance_count, sizeof(FggMaterial));
	//if (p_material_instances == NULL) { return; }
	//memcpy((void*)p_material_instances, src->p_material_instances, src->material_instance_count-1);
	//p_material_instances[src->material_instance_count - 1] = *dst;
}