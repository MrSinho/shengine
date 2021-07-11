#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

ezecsScene editorMakeScene(const FggVkCore core) {

	ezecsScene scene = ezecsCreateScene();

	uint32_t engineCamera = ezecsCreateEntity();
	ezecsAddFggCamera(&scene, engineCamera);

	uint32_t quad = ezecsCreateEntity();
	FggTransform* quadTransform = ezecsAddFggTransform(&scene, quad);
	FggMesh* quadMesh = ezecsAddFggMesh(&scene, quad);
	float vertices[48] = {
		//positions					//UVs		//normals
		1.0f-1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f-1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f-1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f-1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f-1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f-1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
	quadMesh->vertexCount = 48;
	quadMesh->pVertices = vertices;
	FggMaterial* material = ezecsAddFggMaterial(&scene, quad);
	material->pipelineData.shaderStageCount = 2;
	material->pipelineData.pShaderStages = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
	if (material->pipelineData.pShaderStages != NULL) {
		fggCreateShaderStage(core.device, "../Shaders/bin/Triangle.vert.spv", &material->pipelineData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		fggCreateShaderStage(core.device, "../Shaders/bin/Triangle.frag.spv", &material->pipelineData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}
	fggLoadMesh(core, quadMesh);

	uint32_t triangle = ezecsCreateEntity();
	FggMesh* triangleMesh = ezecsAddFggMesh(&scene, triangle);
	float vv[48] = {
		//positions					//UVs		//normals
		1.0f + 1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f + 1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f + 1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f + 1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};
	triangleMesh->vertexCount = 48;
	triangleMesh->pVertices = vv;
	FggMaterial* tmaterial = ezecsAddFggMaterial(&scene, triangle);
	tmaterial->pipelineData = material->pipelineData;
	fggLoadMesh(core, triangleMesh);

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

	FggVkFixedStates fStates = fggFixedStatesInitPrerequisites();
	fggSetFixedStates(core, &fStates);

	fggBuildShader("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	fggBuildShader("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");

	ezecsScene scene = editorMakeScene(core);
	for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
		if (ezecsHasFggMesh(scene, entity)) {
			FggMesh* m = ezecsGetFggMesh(scene, entity);
		}

		if (ezecsHasFggMaterial(scene, entity)) {
			FggMaterial* m = ezecsGetFggMaterial(scene, entity);
			fggSetupGraphicsPipeline(core, fStates, &m->pipelineData);
		}

		if (ezecsHasFggTransform(scene, entity)) {

		}

		if (ezecsHasFggCamera(scene, entity)) {
			FggCamera* camera = ezecsGetFggCamera(scene, entity);

		}
	}

	fggInitCommands(&core);

	FggProjection projection = {0};

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggProjectionUpdate(core.window, &projection);
		fggSceneUpdate(core, projection, scene);
	}

	fggCleanup(&core);

	return 0;
}