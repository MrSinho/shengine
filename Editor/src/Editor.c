#include <FGG_API.h>



#include <stdio.h>
#include <stdlib.h>

int main() {

	FggTime time = { 0 };
	FggVkCore data = fggVkCoreInitPrerequisites(600, 600, "Foggy-Engine Editor");

	fggInitVulkan(&data);
	fggInitSwapchainData(&data);

	fggCreateRenderPass(&data);
	fggSetFramebuffers(&data);
	fggSetSyncObjects(&data);

	FggVkFixedStates fData = fggFixedStatesInitPrerequisites();
	fggSetFixedStates(data, &fData);

	fggBuildShader("../Shaders/src/Charges.vert", "../Shaders/bin/Charges.vert.spv");
	fggBuildShader("../Shaders/src/Charges.frag", "../Shaders/bin/Charges.frag.spv");
	fggBuildShader("../Shaders/src/Triangle.vert", "../Shaders/bin/Triangle.vert.spv");
	fggBuildShader("../Shaders/src/Triangle.frag", "../Shaders/bin/Triangle.frag.spv");

	FggVkPipelineData trianglePipeData = fggVkPipelineDataInitPrerequisitites();
	trianglePipeData.shaderStageCount = 2;
	trianglePipeData.pShaderStages = calloc(2, sizeof(VkPipelineShaderStageCreateInfo));
	if (trianglePipeData.pShaderStages != NULL) {
		fggCreateShaderStage(data.device, "../Shaders/bin/Triangle.vert.spv", &trianglePipeData.pShaderStages[0], VK_SHADER_STAGE_VERTEX_BIT);
		fggCreateShaderStage(data.device, "../Shaders/bin/Triangle.frag.spv", &trianglePipeData.pShaderStages[1], VK_SHADER_STAGE_FRAGMENT_BIT);
	}

	fggSetupGraphicsPipeline(data, fData, &trianglePipeData);

	//SCENE
	ezecsScene scene = { 0 };
	uint32_t triangle = ezecsCreateEntity();
	FggTransform *triangleTransform = ezecsAddFggTransformComponent(&scene, triangle);
	triangleTransform->position[2] = -3.0f;
	

	// MESH AND COMMANDS
	float vertices[48] = {
		//positions					//UVs		//normals
		1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f,  1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
	   -1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f,
		1.0f, -1.0f,  -3.0f,		0.0f, 0.0f,		0.0f, 0.0f, 0.0f
	};

	FggMesh plane = {
		48,				//vertexCount;
		&vertices[0],	//pVertices;
		0,				//indexCount;
		NULL,			//pIndices;
		0,				//vertexBuffer
		0				//vertexBufferMemory			
	};

	fggLoadMesh(data, &plane);
	fggInitCommands(&data);

	FggMeshPushConstants mPushConstants = {0};

	while (fggIsWindowActive(data.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggPushConstantsUpdate(data.window, &mPushConstants);
		fggSceneUpdate(scene);
		fggDraw(&data, &trianglePipeData, mPushConstants, plane, plane.vertexBuffer);
	}

	fggCleanup(&data);
	fggClearBufferMemory(data.device, plane.vertexBuffer, plane.vertexBufferMemory);

	return 0;
}