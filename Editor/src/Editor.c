#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>


void fggSetupBaseMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, FggMaterial* pMaterial) {
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

	fggSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4)*2, &mat.pipelineData);

	fggSetupShaders(core, mat.vertexShaderPath, mat.fragmentShaderPath, &mat.pipelineData);
	
	FggPipelineSetupFlags pipeFlags = FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT | FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	fggSetupGraphicsPipeline(core, fixedStates, pipeFlags, &mat.pipelineData);

	*pMaterial = mat;
}

void fggSetupLineMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, FggMaterial* pMaterial) {
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

	fggSetPushConstants(VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(mat4) * 2, &mat.pipelineData);


	fggSetupShaders(core, mat.vertexShaderPath, mat.fragmentShaderPath, &mat.pipelineData);

	FggPipelineSetupFlags pipeFlags = FGG_PIPELINE_SETUP_PUSH_CONSTANTS_BIT | FGG_PIPELINE_SETUP_UNIFORM_BUFFER_BIT;
	fggSetupGraphicsPipeline(core, fixedStates, pipeFlags, &mat.pipelineData);

	*pMaterial = mat;
}

float* lorenzAttractor(float a, float b, float c, float x, float y, float z) {
	
	float vertex[3];
	float dx, dy, dz;

	dx = (a * (y - x))		* 0.01f;
	dy = (x * (b - z) - y)	* 0.01f;
	dz = (x * y - c * z)	* 0.01f;
	vertex[0] = x + dx;
	vertex[1] = y + dy;
	vertex[2] = z + dz;

	return vertex;
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
	FggFixedStateFlags meshFStateFlags = FGG_FIXED_STATES_POLYGON_MODE_FACE_BIT |
										 FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
										 FGG_FIXED_STATES_VERTEX_NORMALS_BIT | 
										 FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, meshFStateFlags, &meshFStates);
	FggFixedStateFlags lineStateFlags = FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
											FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST |
											FGG_FIXED_STATES_VERTEX_POSITIONS_BIT;
	fggSetFixedStates(core, lineStateFlags, &lineFStates);

	//MATERIALS
	FggMaterial meshMaterial, lineMaterial = { 0 };
	
	fggSetupBaseMaterial(core, meshFStates, &meshMaterial);
	fggSetupLineMaterial(core, lineFStates, &lineMaterial);

	ezecsScene scene = { 0 };
	ezecsCreateScene(scene);

	//camera
	uint32_t camEntity = ezecsCreateEntity();
	FggCamera cam = {
		45.0f,								//fov;
		0.1f,								//nc;
		150.0f,								//fc;
		{0.0f},								//projection;
		{0.0f},								//view;
		FGG_CAMERA_SETUP_FREE_FLIGHT_BIT,	//flags
	};
	ezecsSetFggCamera(scene, &cam, camEntity);
	FggTransform* camTransform = ezecsAddFggTransform(scene, camEntity);
	camTransform->position[2] = 2.0f;

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
	handTransform->scale[0] = 0.5f;
	handTransform->scale[1] = 0.5f;
	handTransform->scale[2] = 0.5f;
	handTransform->position[0] = -3.5f;
	handTransform->position[1] = -1.85f;
	handTransform->position[2] = -6.0f;
	
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
	fggSetupBaseMaterial(core, meshFStates, &newmat);
	ezecsSetFggMaterial(scene, &newmat, lucy);
	lucyTransform->scale[0] = 1.0f;
	lucyTransform->scale[1] = 1.0f;
	lucyTransform->scale[2] = 1.0f;
	lucyTransform->position[0] = 2.3f;
	lucyTransform->position[1] = 1.0f;
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
	fggSetupBaseMaterial(core, meshFStates, &newmat1);
	ezecsSetFggMaterial(scene, &newmat1, text);
	textTransform->scale[0] = 1.0f;
	textTransform->scale[1] = 1.0f;
	textTransform->scale[2] = 1.0f;

	//line
	//float vertices[15] = {
	//	0.0f, -1.0f, 0.0f, 
	//	1.0f,  1.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f,
	//	0.0f, -0.8f, 0.0f,
	//	0.0f,  0.0f, 0.0f,
	//};
	//uint32_t line = ezecsCreateEntity();
	//FggMesh* lineMesh = ezecsAddFggMesh(scene, line);
	//lineMesh->flags = FGG_MESH_SETUP_DYNAMIC_MESH;
	//lineMesh->vertexCount = sizeof(vertices) / sizeof(float);
	//lineMesh->pVertices = calloc(lineMesh->vertexCount, sizeof(float));
	//fggGenerateGraphIndices(lineMesh);
	//
	//if (lineMesh->pVertices == NULL) { return EXIT_FAILURE; }
	//for (uint32_t i = 0; i < lineMesh->vertexCount; i++) {
	//	lineMesh->pVertices[i] = vertices[i];
	//}
	//ezecsSetFggMaterial(scene, &lineMaterial, line);
	//FggTransform* lineTransform = ezecsAddFggTransform(scene, line);
	//lineTransform->scale[0] = 1.0f;
	//lineTransform->scale[1] = 1.0f;
	//lineTransform->scale[2] = 1.0f;

	//graph 
	uint32_t graph = ezecsCreateEntity();
	FggMesh* graphMesh = ezecsAddFggMesh(scene, graph);
	graphMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	graphMesh->vertexCount = 5000 * 3;
	graphMesh->pVertices = calloc(graphMesh->vertexCount, sizeof(float));
	if (graphMesh->pVertices == NULL) { return EXIT_FAILURE; }
	for (uint32_t i = 0; i < graphMesh->vertexCount; i+=3) {
		float* vertex;
		if (i == 0) {
			vertex = lorenzAttractor(10.0f, 28.0f, 2.66f, 0.01f, 0.0f, 0.0f);
		}
		else {
			vertex = lorenzAttractor(10.0f, 28.0f, 2.66f, graphMesh->pVertices[i-3], graphMesh->pVertices[i-2], graphMesh->pVertices[i-1]);
		}
		graphMesh->pVertices[i]   = vertex[0];
		graphMesh->pVertices[i+1] = vertex[1];
		graphMesh->pVertices[i+2] = vertex[2];
	}
	fggGenerateGraphIndices(graphMesh);
	FggMaterial graphMat = { 0 };
	fggSetupLineMaterial(core, lineFStates, &graphMat);
	ezecsSetFggMaterial(scene, &graphMat, graph);
	FggTransform* graphTransform = ezecsAddFggTransform(scene, graph);
	graphTransform->rotation[1] = 180.0f;
	graphTransform->scale[0] = 1.0f;
	graphTransform->scale[1] = 1.0f;
	graphTransform->scale[2] = 1.0f;

	fggSceneInit(core, scene);
	fggInitCommands(&core);

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
		
		fggGetCursorPosition(core.window, &core.window.cursorPosX, &core.window.cursorPosY);

		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
			
		//lineMesh->pVertices[0] = (float)sin(time.now);
		//lineMesh->pVertices[12] = (float)sin(time.now);
		handTransform->rotation[1] += 50.0f * time.deltaTime;
		lucyTransform->rotation[1] += 25.0f * time.deltaTime;
		textTransform->rotation[1] -= 100 * time.deltaTime;

		fggSceneUpdate(core, time, scene);
	
		fggFrameEnd(core, imageIndex);
	}
	
	plyFree(&handply);
	plyFree(&lucyply);
	fggSceneRelease(core, scene);
	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}