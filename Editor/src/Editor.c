#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void fggSetupBaseMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, void** ppPushConstants, FggMaterial* pMaterial) {
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	
	FggMaterial mat = {
		"../Shaders/bin/Mesh.vert.spv",	//vertexShaderPath;
		"../Shaders/bin/Mesh.frag.spv",	//fragmentShaderPath;	
		0,										//pipelineData;
	};

	fggAllocateUniformBufferData(core, sizeof(mat4), &mat.pipelineData);
	fggDescriptorSetLayout(core, 0, VK_SHADER_STAGE_VERTEX_BIT, &mat.pipelineData);
	fggCreateDescriptorPool(core, &mat.pipelineData);
	fggAllocateDescriptorSets(core, &mat.pipelineData);

	fggSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4)*2, ppPushConstants, &mat.pipelineData);


	fggSetupShaders(core, mat.vertexShaderPath, mat.fragmentShaderPath, &mat.pipelineData);
	
	FggPipelineSetupFlags pipeFlags = FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT | FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	fggSetupGraphicsPipeline(core, fixedStates, pipeFlags, &mat.pipelineData);

	*pMaterial = mat;
}

void fggSetupLineMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, void** ppPushConstants, FggMaterial* pMaterial) {
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Line.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Line.frag.spv");

	FggMaterial mat = {
		"../Shaders/bin/Line.vert.spv",	//vertexShaderPath;
		"../Shaders/bin/Line.frag.spv",	//fragmentShaderPath;	
		0,										//pipelineData;
	};

	fggAllocateUniformBufferData(core, sizeof(mat4), &mat.pipelineData);
	fggDescriptorSetLayout(core, 0, VK_SHADER_STAGE_VERTEX_BIT, &mat.pipelineData);
	fggCreateDescriptorPool(core, &mat.pipelineData);
	fggAllocateDescriptorSets(core, &mat.pipelineData);

	fggSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4) * 2, ppPushConstants, &mat.pipelineData);


	fggSetupShaders(core, mat.vertexShaderPath, mat.fragmentShaderPath, &mat.pipelineData);

	FggPipelineSetupFlags pipeFlags = FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT | FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	fggSetupGraphicsPipeline(core, fixedStates, pipeFlags, &mat.pipelineData);

	*pMaterial = mat;
}

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(720, 480, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates meshFStates, lineFStates = { 0 };
	FggFixedStateFlags meshFStateFlags = FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
										 FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST |
										 FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
										 FGG_FIXED_STATES_VERTEX_NORMALS_BIT | 
										 FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, meshFStateFlags, &meshFStates);
	FggFixedStateFlags lineStateFlags = FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
										FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST |
										FGG_FIXED_STATES_VERTEX_POSITIONS_BIT;
	fggSetFixedStates(core, lineStateFlags, &lineFStates);

	//MATERIALS
	FggMaterial baseMaterial, lineMaterial = { 0 };
	
	mat4 pConst[2] = { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT };
	fggSetupBaseMaterial(core, meshFStates, (void**)pConst, &baseMaterial);
	fggSetupLineMaterial(core, lineFStates, (void**)pConst, &lineMaterial);

	ezecsScene scene = { 0 };
	ezecsCreateScene(scene);

	//PRISM
	PlyFileData prismply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordHand.ply", &prismply, 0);
	uint32_t prism = ezecsCreateEntity();
	FggTransform* prismTransform = ezecsAddFggTransform(scene, prism);
	FggMesh* prismMesh = ezecsAddFggMesh(scene, prism);
	prismMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	prismMesh->vertexCount = prismply.vertexCount * prismply.vertexStride;
	prismMesh->pVertices = prismply.pVertices;
	prismMesh->indexCount = prismply.indexCount;
	prismMesh->pIndices = prismply.pIndices;
	ezecsSetFggMaterial(scene, &baseMaterial, prism);
	prismTransform->scale[0] = 1.0f;
	prismTransform->scale[1] = 1.0f;
	prismTransform->scale[2] = 1.0f;
	prismTransform->position[0] = 0.0f;
	prismTransform->position[2] = -3.0f;

	//prismTransform->rotation[1] = -100.0f;

	//RAYS
	float vertices[12] = {
	-1.0f, -1.0f, 0.0f, 
	1.0f,  1.0f, 0.0f, 
	-1.0f,  1.0f, 0.0f,
	};
	uint32_t ray = ezecsCreateEntity();
	FggMesh* rayMesh = ezecsAddFggMesh(scene, ray);
	rayMesh->flags = FGG_MESH_SETUP_DYNAMIC_MESH;
	rayMesh->vertexCount = sizeof(vertices) / sizeof(float);
	rayMesh->pVertices = calloc(rayMesh->vertexCount, sizeof(uint32_t));
	if (rayMesh->pVertices == NULL) { return EXIT_FAILURE; }
	for (uint32_t i = 0; i < rayMesh->vertexCount; i++) {
		rayMesh->pVertices[i] = vertices[i];
	}
	ezecsSetFggMaterial(scene, &lineMaterial, ray);
	FggTransform* rayTransform = ezecsAddFggTransform(scene, ray);
	rayTransform->scale[0] = 1.0f;
	rayTransform->scale[1] = 1.0f;
	rayTransform->scale[2] = 1.0f;

	fggSceneInit(core, scene);
	fggInitCommands(&core);

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
	
		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
	
		fggSetProjection(core.window, pConst[0]);

		fggSetView(pConst[1]);

		//rayMesh->pVertices[0] *= (float)sin((float)time.now);
		prismTransform->position[2] = -0.5*(float)sin((float)time.now);
		prismTransform->rotation[1] += 1.0f;
		fggSceneUpdate(core, scene);
	
		fggFrameEnd(core, imageIndex);
	}
	
	//plyFree(&prismply);
	fggSceneRelease(core, scene);
	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}