#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void editorMakeScene(const FggVkCore core, const FggMaterial mat, ezecsScene scene) {

	ezecsCreateScene(scene);

	PlyFileData geometryply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordLucy.ply", &geometryply, 0);

	uint32_t quad = ezecsCreateEntity();
	FggTransform* quadTransform = ezecsAddFggTransform(scene, quad);
	FggMesh* quadMesh = ezecsAddFggMesh(scene, quad);
	
	quadMesh->vertexCount = geometryply.vertexCount * geometryply.vertexStride;
	quadMesh->pVertices = geometryply.pVertices;
	quadMesh->indexCount = geometryply.indexCount;
	quadMesh->pIndices = geometryply.pIndices;
	const FggMaterial *m = ezecsSetFggMaterial(scene, &mat, quad);
	fggAllocateMeshVertexData(core, quadMesh);
	fggAllocateMeshIndexData(core, quadMesh);

	plyFree(&geometryply);
}

FggMaterial fggSetupMaterial(const FggVkCore core, void** ppPushConstants) {

	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	
	FggMaterial baseMaterial = {
		0,					//pipelineData
		0,					//pushConstantRange;
		ppPushConstants,	//pPushConstantsData;
	};
	baseMaterial.pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	baseMaterial.pushConstantRange.offset = 0;
	baseMaterial.pushConstantRange.size = sizeof(mat4)*2;
	baseMaterial.ppPushConstantsData = ppPushConstants;
	fggInitPipelineData(core, "../Shaders/bin/Mesh.vert.spv", "../Shaders/bin/Mesh.frag.spv", &baseMaterial.pipelineData);

	return baseMaterial;
}

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(600, 600, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates fStates = { 0 };
	fggSetFixedStates(core, &fStates);

							//projection		//view
	mat4 pConst[2] = { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT };
	FggMaterial baseMaterial = fggSetupMaterial(core, (void*)&pConst);
	ezecsScene scene;
	editorMakeScene(core, baseMaterial, scene);
	fggSceneInit(core, fStates, scene);

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

	fggCleanup(&core);
	fggReleaseShaderStages(baseMaterial.pipelineData);

	return 0;
}