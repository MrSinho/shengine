#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void editorMakeScene(const FggVkCore core, const FggMaterial mat, void* scene) {

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

void fggSetupMaterial(const FggVkCore core, void** ppPushConstants, FggMaterial* pMaterial) {

	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	
	pMaterial->pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	pMaterial->pushConstantRange.offset = 0;
	pMaterial->pushConstantRange.size = sizeof(mat4)*2;
	pMaterial->ppPushConstantsData = ppPushConstants;
	fggInitPipelineData(core, "../Shaders/bin/Mesh.vert.spv", "../Shaders/bin/Mesh.frag.spv", &pMaterial->pipelineData);

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
	FggMaterial baseMaterial = { 0, 0, NULL };
	fggSetupMaterial(core, (void*)&pConst, &baseMaterial);

	ezecsScene scene;
	editorMakeScene(core, baseMaterial, (void*)scene);
	//fggSceneInit(core, fStates, scene);

	FggMaterial* m = ezecsGetFggMaterial((void*)scene, 0);

	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		if (ezecsHasFggMesh((void*)scene, entity)) {

		}

		if (ezecsHasFggMaterial((void*)scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial((void*)scene, entity);
			fggSetupGraphicsPipeline(core, fStates, m->pushConstantRange, &m->pipelineData);
		}

		if (ezecsHasFggTransform((void*)scene, entity)) {

		}
	}

	fggInitCommands(&core);


	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
	
		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
	
		fggSetProjection(core.window, pConst[0]);
		fggSetView(pConst[1]);

		fggSceneUpdate(core, (void*)scene);
	
		fggFrameEnd(core, imageIndex);
	}

	fggCleanup(&core);
	fggReleaseShaderStages(baseMaterial.pipelineData);

	return 0;
}