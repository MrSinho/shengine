#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

#include "FDL.h"

void fggSetupMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, void** ppPushConstants, FggMaterial* pMaterial) {
	//fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	//fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	
	FggMaterial mat = {
		"../Shaders/bin/Mesh.vert.spv",	//vertexShaderPath;
		"../Shaders/bin/Mesh.frag.spv",	//fragmentShaderPath;	
		VK_SHADER_STAGE_VERTEX_BIT,		//pushConstantsShaderStageFlags;
		sizeof(mat4) * 2,				//pushConstantsSize;
		0,								//pushConstantsOffset;
		ppPushConstants,				//ppPushConstantsData;
		0,								//pipelineData;
		0								//pushConstantRange;
	};

	fggSetPushConstants(mat.pushConstantsShaderStageFlags, mat.pushConstantsOffset, mat.pushConstantsSize, mat.ppPushConstantsData, &mat.pushConstantRange);
	fggInitPipelineData(core, "../Shaders/bin/Mesh.vert.spv", "../Shaders/bin/Mesh.frag.spv", &mat.pipelineData);
	fggSetupGraphicsPipeline(core, fixedStates, mat.pushConstantRange, &mat.pipelineData);

	*pMaterial = mat;
}

int main() {

	fdlBuild("../../scene.fdl");

#if 0
	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(600, 600, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates fixedStates = { 0 };
	fggSetFixedStates(core, &fixedStates);

							//projection		//view
	mat4 pConst[2] = { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT };
	FggMaterial baseMaterial = { 0 };
	fggSetupMaterial(core, fixedStates, (void*)&pConst, &baseMaterial);

	ezecsScene scene = { 0 };
	ezecsCreateScene(scene);
	fggImport("../bin/scene.fgg", scene);


	//PlyFileData geometryply = { 0 };
	//plyLoadFile("../Assets/Meshes/stanfordHand.ply", &geometryply, 0);
	//uint32_t quad = ezecsCreateEntity();
	//FggTransform* quadTransform = ezecsAddFggTransform(scene, quad);
	//FggMesh* geometryMesh = ezecsAddFggMesh(scene, quad);
	//geometryMesh->vertexCount = geometryply.vertexCount * geometryply.vertexStride;
	//geometryMesh->pVertices		= geometryply.pVertices;
	//geometryMesh->indexCount = geometryply.indexCount;
	//geometryMesh->pIndices = geometryply.pIndices;
	//FggMesh* mesh = ezecsGetFggMesh(scene, 0);
	//FggMaterial* mat = ezecsGetFggMaterial(scene, 0);
	ezecsSetFggMaterial(scene, &baseMaterial, 0);
	
	fggSceneInit(core, fixedStates, scene);
	fggInitCommands(&core);

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
	
		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
	
		fggSetProjection(core.window, pConst[0]);

		fggSetView(pConst[1]);

		fggSceneUpdate(core, scene);
	
		fggFrameEnd(core, imageIndex);
	}
	
	fggExport("../bin/scene.fgg", scene);


	//plyFree(&geometryply);
	fggSceneRelease(core, scene);
	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
#endif
}