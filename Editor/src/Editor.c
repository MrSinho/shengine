#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void fggSetupBaseMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, void** ppPushConstants, FggMaterial* pMaterial) {
#ifndef NDEBUG
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
#endif
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
#ifndef NDEBUG
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Line.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Line.frag.spv");
#endif

	FggMaterial mat = {
		"../Shaders/bin/Line.vert.spv",	//vertexShaderPath;
		"../Shaders/bin/Line.frag.spv",	//fragmentShaderPath;	
		0,								//pipelineData;
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
	FggVkCore core = fggVkCoreInitPrerequisites(1920, 1080, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates meshFStates, lineFStates = { 0 };
	FggFixedStateFlags meshFStateFlags = FGG_FIXED_STATES_POLYGON_MODE_FACE_BIT |
										 FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
										 FGG_FIXED_STATES_VERTEX_NORMALS_BIT | 
										 FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, meshFStateFlags, &meshFStates);
	FggFixedStateFlags triangleStateFlags = FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
											FGG_FIXED_STATES_VERTEX_POSITIONS_BIT;
	fggSetFixedStates(core, triangleStateFlags, &lineFStates);

	//MATERIALS
	FggMaterial meshMaterial, lineMaterial = { 0 };
	
	mat4 pConst[2] = { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT };
	fggSetupBaseMaterial(core, meshFStates, (void**)pConst, &meshMaterial);
	fggSetupLineMaterial(core, lineFStates, (void**)pConst, &lineMaterial);

	ezecsScene scene = { 0 };
	ezecsCreateScene(scene);

	//hand
	PlyFileData handply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordHand.ply", &handply, 0);
	uint32_t hand = ezecsCreateEntity();
	FggTransform* handTransform = ezecsAddFggTransform(scene, hand);
	FggMesh* handMesh = ezecsAddFggMesh(scene, hand);
	handMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	handMesh->vertexCount = handply.vertexCount * handply.vertexStride;
	handMesh->pVertices = handply.pVertices;
	handMesh->indexCount = handply.indexCount;
	handMesh->pIndices = handply.pIndices;
	ezecsSetFggMaterial(scene, &meshMaterial, hand);
	handTransform->scale[0] = 1.0f;
	handTransform->scale[1] = 1.0f;
	handTransform->scale[2] = 1.0f;
	handTransform->position[0] = 0.0f;
	handTransform->position[1] = -1.3f;
	handTransform->position[2] = -5.0f;
	
	//lucy
	PlyFileData lucyply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordLucy.ply", &lucyply, 0);
	uint32_t lucy = ezecsCreateEntity();
	FggTransform* lucyTransform = ezecsAddFggTransform(scene, lucy);
	FggMesh* lucyMesh = ezecsAddFggMesh(scene, lucy);
	lucyMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	lucyMesh->vertexCount = lucyply.vertexCount * lucyply.vertexStride;
	lucyMesh->pVertices = lucyply.pVertices;
	lucyMesh->indexCount = lucyply.indexCount;
	lucyMesh->pIndices = lucyply.pIndices;
	FggMaterial newmat = { 0 };
	fggSetupBaseMaterial(core, meshFStates, (void**)pConst, &newmat);
	ezecsSetFggMaterial(scene, &newmat, lucy);
	lucyTransform->scale[0] = 1.0f;
	lucyTransform->scale[1] = 1.0f;
	lucyTransform->scale[2] = 1.0f;
	lucyTransform->position[0] = 2.0f;
	lucyTransform->position[1] = 0.0f;
	lucyTransform->position[2] = -2.0f;
	
	//text
	PlyFileData textply = { 0 };
	plyLoadFile("../Assets/Meshes/text.ply", &textply, 0);
	uint32_t text = ezecsCreateEntity();
	FggTransform* textTransform = ezecsAddFggTransform(scene, text);
	FggMesh* textMesh = ezecsAddFggMesh(scene, text);
	textMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	textMesh->vertexCount = textply.vertexCount * textply.vertexStride;
	textMesh->pVertices = textply.pVertices;
	textMesh->indexCount = textply.indexCount;
	textMesh->pIndices = textply.pIndices;
	FggMaterial newmat1 = { 0 };
	fggSetupBaseMaterial(core, meshFStates, (void**)pConst, &newmat1);
	ezecsSetFggMaterial(scene, &newmat1, text);
	textTransform->scale[0] = 1.0f;
	textTransform->scale[1] = 1.0f;
	textTransform->scale[2] = 1.0f;

	//triangle
	float vertices[12] = {
		0.0f, -1.0f, 0.0f, 
		1.0f,  1.0f, 0.0f, 
		-1.0f,  1.0f, 0.0f,
	};
	uint32_t triangle = ezecsCreateEntity();
	FggMesh* triangleMesh = ezecsAddFggMesh(scene, triangle);
	triangleMesh->flags = FGG_MESH_SETUP_DYNAMIC_MESH;
	triangleMesh->vertexCount = sizeof(vertices) / sizeof(float);
	triangleMesh->pVertices = calloc(triangleMesh->vertexCount, sizeof(uint32_t));
	if (triangleMesh->pVertices == NULL) { return EXIT_FAILURE; }
	for (uint32_t i = 0; i < triangleMesh->vertexCount; i++) {
		triangleMesh->pVertices[i] = vertices[i];
	}
	ezecsSetFggMaterial(scene, &lineMaterial, triangle);
	FggTransform* triangleTransform = ezecsAddFggTransform(scene, triangle);
	triangleTransform->scale[0] = 1.0f;
	triangleTransform->scale[1] = 1.0f;
	triangleTransform->scale[2] = 1.0f;

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

		triangleMesh->pVertices[0] = (float)sin((float)time.now);
		handTransform->rotation[1] += 50.0f * time.deltaTime;
		lucyTransform->rotation[1] += 25.0f * time.deltaTime;
		textTransform->rotation[1] -= 100 * time.deltaTime;

		fggSceneUpdate(core, scene);
	
		fggFrameEnd(core, imageIndex);
	}
	
	//plyFree(&handply);
	fggSceneRelease(core, scene);
	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}