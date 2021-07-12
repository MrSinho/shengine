#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

ezecsScene editorMakeScene(const FggVkCore core) {

	ezecsScene scene = ezecsCreateScene();

	uint32_t engineCamera = ezecsCreateEntity();
	ezecsAddFggCamera(&scene, engineCamera);

	uint32_t quad = ezecsCreateEntity();
	FggTransform* t= ezecsAddFggTransform(&scene, quad);
	FggMesh* m = ezecsAddFggMesh(&scene, quad);
	float vertices[48] = {
		1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f - 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f - 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
	m->vertexCount = 48;
	m->pVertices = vertices;
	FggMaterial* mat = ezecsAddFggMaterial(&scene, quad);
	mat->pipelineData.shaderStageCount = 2;
	mat->pipelineData.pShaderStages = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
	if (mat->pipelineData.pShaderStages != NULL) {
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.vert.spv", &mat->pipelineData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.frag.spv", &mat->pipelineData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}
	fggLoadMesh(core, m);

	uint32_t triangle = ezecsCreateEntity();
	FggTransform* tt = ezecsAddFggTransform(&scene, triangle);
	FggMesh* tm = ezecsAddFggMesh(&scene, triangle);
	float tvertices[48] = {
		1.0f + 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f,  1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f, -1.0f,  -4.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	};
	tm->vertexCount = 24;
	tm->pVertices = tvertices;
	FggMaterial* tmat = ezecsAddFggMaterial(&scene, triangle);
	tmat->pipelineData.shaderStageCount = 2;
	tmat->pipelineData.pShaderStages = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
	if (tmat->pipelineData.pShaderStages != NULL) {
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.vert.spv", &tmat->pipelineData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		fggCreateShaderStage(core.device, "../Shaders/bin/Mesh.frag.spv", &tmat->pipelineData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}
	fggLoadMesh(core, tm);

	return scene;
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

	fggBuildShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggBuildShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");

	ezecsScene scene = editorMakeScene(core);
	fggSceneInit(core, fStates, scene);
	fggInitCommands(&core);

	mat4 projection = GLM_MAT4_IDENTITY_INIT;

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);

		fggFrameReset(core);

		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);

		fggSetProjection(core.window, &projection);
		fggSceneUpdate(core, projection, scene);

		fggFrameEnd(core, imageIndex);
	}

	fggCleanup(&core);

	return 0;
}