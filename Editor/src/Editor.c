#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

ezecsScene* editorMakeScene(const FggVkCore core, FggMaterial mat) {

	ezecsScene* scene = ezecsCreateScene();

	uint32_t engineCamera = ezecsCreateEntity();
	ezecsAddFggCamera(scene, engineCamera);

	uint32_t quad = ezecsCreateEntity();
	FggTransform* quadTransform = ezecsAddFggTransform(scene, quad);
	FggMesh* quadMesh = ezecsAddFggMesh(scene, quad);
	float vertices[48] = {
		1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
	quadMesh->vertexCount = 48;
	quadMesh->pVertices = vertices;
	FggMaterial* quadMat = ezecsAddFggMaterial(scene, quad);
	*quadMat = mat;
	fggLoadMesh(core, quadMesh);

	uint32_t triangle = ezecsCreateEntity();
	FggTransform* triangleTransform = ezecsAddFggTransform(scene, triangle);
	FggMesh* triangleMesh = ezecsAddFggMesh(scene, triangle);
	float tvertices[48] = {
		1.0f + 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	};
	triangleMesh->vertexCount = 24;
	triangleMesh->pVertices = tvertices;
	FggMaterial* triangleMat = ezecsAddFggMaterial(scene, triangle);
	*triangleMat = mat;
	fggLoadMesh(core, triangleMesh);

	return scene;
}

FggMaterial fggSetupMaterial(const FggVkCore core) {
	
	fggBuildShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggBuildShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");

	FggMaterial baseMaterial = { 0 };
	baseMaterial.pipelineData.shaderStageCount = 2;
	baseMaterial.pipelineData.pShaderStages = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
	if (baseMaterial.pipelineData.pShaderStages != NULL) {
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.vert.spv", &baseMaterial.pipelineData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.frag.spv", &baseMaterial.pipelineData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}
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

	FggMaterial mat = fggSetupMaterial(core);
	ezecsScene* scene = editorMakeScene(core, mat);
	
	fggSceneInit(core, fStates, scene);
	fggInitCommands(&core);

	mat4 projection = GLM_MAT4_IDENTITY_INIT;

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);

		fggFrameReset(core);

		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);

		fggSetProjection(core.window, projection);
		fggSceneUpdate(core, projection, scene);

		fggFrameEnd(core, imageIndex);
	}

	fggCleanup(&core);

	return 0;
}