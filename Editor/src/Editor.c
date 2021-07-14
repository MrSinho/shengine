#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void editorMakeScene(const FggVkCore core, const FggMaterial mat, ezecsScene scene) {

	ezecsCreateScene(scene);

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
	const FggMaterial *m = ezecsSetFggMaterial(scene, &mat, quad);
	fggAllocateMeshData(core, quadMesh);

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
	const FggMaterial* matr = ezecsSetFggMaterial(scene, &mat, triangle);
	fggAllocateMeshData(core, triangleMesh);
}

FggMaterial fggSetupMaterial(const FggVkCore core, const void* pPushConstants) {

	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	
	FggMaterial baseMaterial = {
		0,				//pipelineData
		{0},			//pushConstantRange;
		pPushConstants,		//pPushConstantsData;
	};
	baseMaterial.pushConstantRange.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
	baseMaterial.pushConstantRange.offset = 0;
	baseMaterial.pushConstantRange.size = sizeof(mat4);
	baseMaterial.pPushConstantsData = pPushConstants;
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

	mat4 projection = GLM_MAT4_IDENTITY_INIT;
	void* pushConstants[1] = { (void*)projection };
	FggMaterial baseMaterial = fggSetupMaterial(core, pushConstants[0]);
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
	
		fggSetProjection(core.window, projection);
	
		fggSceneUpdate(core, scene);
	
		fggFrameEnd(core, imageIndex);
	}

	fggCleanup(&core);
	fggReleaseShaderStages(baseMaterial.pipelineData);

	return 0;
}