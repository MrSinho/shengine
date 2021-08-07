#include "fggMaterial.h" 

void fggSetupMaterial(const FggVkCore core, const char* vPath, const char* fPath, const uint32_t uniformSize, const VkShaderStageFlags uniformStage, const uint32_t pConstSize, const VkShaderStageFlags pConstStage, const FggVkFixedStates fixedStates, FggMaterial* pMaterial) {
	FggMaterial mat = {
		vPath,			//vertexShaderPath;
		fPath,			//fragmentShaderPath;	
		&fixedStates,	//pFixedStates
		0,				//pipelineData;
	};

	FggPipelineSetupFlags pipeFlags = 0;

	if (uniformSize != NULL && uniformStage != 0) {	
		fggAllocateUniformBufferData(core, uniformSize, &mat.pipelineData);
		fggDescriptorSetLayout(core, 0, uniformStage, &mat.pipelineData);
		fggCreateDescriptorPool(core, &mat.pipelineData);
		fggAllocateDescriptorSets(core, &mat.pipelineData);
		pipeFlags |= FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	}
	
	if (pConstSize != NULL && pConstStage != 0) {
		fggSetPushConstants(pConstStage, 0, pConstSize, &mat.pipelineData);
		pipeFlags |= FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT;
	}

	fggSetupShaders(core, mat.vertexShaderPath, mat.fragmentShaderPath, &mat.pipelineData);

	fggSetupGraphicsPipeline(core, fixedStates, pipeFlags, &mat.pipelineData);

	*pMaterial = mat;
}

void fggCreateMaterialInstance(const FggVkCore core, const FggMaterial src, FggMaterial* dst) {
	fggSetupMaterial(core,
		src.vertexShaderPath, src.fragmentShaderPath,
		src.pipelineData.uniformBufferSize, src.pipelineData.descriptorSetLayoutBinding.stageFlags,
		src.pipelineData.pushConstantRange.size, src.pipelineData.pushConstantRange.stageFlags,
		*src.pFixedStates, dst);
}