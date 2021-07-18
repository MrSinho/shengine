#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

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
	FggMaterial baseMaterial = { 0 };
	fggSetupMaterial(core, (void*)&pConst, &baseMaterial);

	ezecsScene scene;
	ezecsCreateScene(scene);

	PlyFileData geometryply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordHand.ply", &geometryply, 0);
	uint32_t quad = ezecsCreateEntity();
	FggTransform* quadTransform = ezecsAddFggTransform(scene, quad);
	FggMesh* geometryMesh = ezecsAddFggMesh(scene, quad);
	geometryMesh->vertexCount = geometryply.vertexCount * geometryply.vertexStride;
	geometryMesh->pVertices = geometryply.pVertices;
	geometryMesh->indexCount = geometryply.indexCount;
	geometryMesh->pIndices = geometryply.pIndices;
	ezecsSetFggMaterial(scene, &baseMaterial, quad);
	fggAllocateMeshVertexData(core, geometryMesh);
	fggAllocateMeshIndexData(core,  geometryMesh);
	plyFree(&geometryply);

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