#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SERVOS
#define RANDOM
#define LORENZ

#ifdef RANDOM
#define LORENZ
#endif

#ifdef LORENZ

float* lorenzAttractorVertex(float a, float b, float c, float dTime, float x, float y, float z) {
	
	float* vertex = calloc(3, sizeof(float));
	if (vertex == NULL) { return NULL; }
	float dx, dy, dz;

	dx = (a * (y - x))		* dTime;
	dy = (x * (b - z) - y)	* dTime;
	dz = (x * y - c * z)	* dTime;
	vertex[0] = x + dx;
	vertex[1] = y + dy;
	vertex[2] = z + dz;

	return vertex;
}

void lorenzAttractor(float a, float b, float c, float dTime, FggMesh* mesh) {
	if (mesh->pVertices != NULL) {
		free(mesh->pVertices);
	}
	if (mesh->pIndices != NULL) {
		free(mesh->pIndices); 
	}
	mesh->pVertices = calloc(mesh->vertexCount, sizeof(float));
	if (mesh->pVertices == NULL) { return; }
	for (uint32_t i = 0; i < mesh->vertexCount; i += 3) {
		float* hvertex;
		float vertex[3];
		if (i == 0) {
			hvertex = lorenzAttractorVertex(10.0f, 28.0f, 2.66f, dTime, 0.01f, 0.0f, 0.0f);
		}
		else {
			hvertex = lorenzAttractorVertex(10.0f, 28.0f, 2.66f, dTime, mesh->pVertices[i - 3], mesh->pVertices[i - 2], mesh->pVertices[i - 1]);
		}
		memcpy(vertex, hvertex, sizeof(float) * 3);
		free(hvertex);
		mesh->pVertices[i]		= vertex[0];
		mesh->pVertices[i + 1]	= vertex[1];
		mesh->pVertices[i + 2]	= vertex[2];
	}
	fggGenerateGraphIndices(mesh);
}

#endif

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(720, 480, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates meshFStates, wireframeFStates, lineFStates = { 0 };
	FggFixedStateFlags meshFStateFlags = FGG_FIXED_STATES_POLYGON_MODE_FACE_BIT |
										 FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
										 FGG_FIXED_STATES_VERTEX_NORMALS_BIT | 
										 FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, meshFStateFlags, &meshFStates);
	FggFixedStateFlags wireframeFStateFlags =	FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
												FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
												FGG_FIXED_STATES_VERTEX_NORMALS_BIT |
												FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, wireframeFStateFlags, &wireframeFStates);
	FggFixedStateFlags lineStateFlags = FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
											FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST |
											FGG_FIXED_STATES_VERTEX_POSITIONS_BIT;
	fggSetFixedStates(core, lineStateFlags, &lineFStates);

#ifndef NDEBUG
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	fggCompileGLSLShader("../Shaders/src/Line.vert", "../Shaders/bin/Line.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Line.frag", "../Shaders/bin/Line.frag.spv");
#endif // NDEBUG

	//MATERIALS
	FggMaterial meshMaterial, wireframeMaterial, lineMaterial;
	
	fggSetupMaterial(core, 
		"../Shaders/bin/Mesh.vert.spv", "../Shaders/bin/Mesh.frag.spv", 
		sizeof(mat4), VK_SHADER_STAGE_VERTEX_BIT,
		sizeof(mat4) * 2, VK_SHADER_STAGE_VERTEX_BIT,
		meshFStates, &meshMaterial
		);

	fggSetupMaterial(core,
		"../Shaders/bin/Mesh.vert.spv", "../Shaders/bin/Mesh.frag.spv",
		sizeof(mat4), VK_SHADER_STAGE_VERTEX_BIT,
		sizeof(mat4) * 2, VK_SHADER_STAGE_VERTEX_BIT,
		wireframeFStates, &wireframeMaterial
	);

	fggSetupMaterial(core,
		"../Shaders/bin/Line.vert.spv", "../Shaders/bin/Line.frag.spv",
		sizeof(mat4), VK_SHADER_STAGE_VERTEX_BIT,
		sizeof(mat4) * 2, VK_SHADER_STAGE_VERTEX_BIT,
		lineFStates, &lineMaterial
	);
	
#ifdef RANDOM
	FggMaterial handMat, lucyMat, textMat, planeMat;
	fggCreateMaterialInstance(core, meshMaterial, &handMat);
	fggCreateMaterialInstance(core, meshMaterial, &lucyMat);
	fggCreateMaterialInstance(core, meshMaterial, &textMat);
	fggCreateMaterialInstance(core, meshMaterial, &planeMat);
#endif // RANDOM

#ifdef SERVOS
	FggMaterial servo0Mat, cross0Mat, servo1Mat, cross1Mat = { 0 };
	fggCreateMaterialInstance(core, wireframeMaterial, &servo0Mat);
	fggCreateMaterialInstance(core, wireframeMaterial, &cross0Mat);
	fggCreateMaterialInstance(core, wireframeMaterial, &servo1Mat);
	fggCreateMaterialInstance(core, wireframeMaterial, &cross1Mat);
#endif // SERVOS

#ifdef LORENZ
	FggMaterial lorenzMaterial = { 0 };
	fggCreateMaterialInstance(core, lineMaterial, &lorenzMaterial);
#endif // LORENZ

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

#ifdef RANDOM
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
	ezecsSetFggMaterial(scene, &handMat, hand);
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
	ezecsSetFggMaterial(scene, &lucyMat, lucy);
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
	ezecsSetFggMaterial(scene, &textMat, text);
	textTransform->scale[0] = 1.0f;
	textTransform->scale[1] = 1.0f;
	textTransform->scale[2] = 1.0f;

	//plane
	PlyFileData planePly = { 0 };
	plyLoadFile("../Assets/Meshes/plane.ply", &planePly, 0);
	uint32_t plane = ezecsCreateEntity();
	FggTransform* planeTransform = ezecsAddFggTransform(scene, plane);
	FggMesh* planeMesh = ezecsAddFggMesh(scene, plane);
	planeMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	planeMesh->vertexCount = planePly.vertexCount * planePly.vertexStride;
	planeMesh->pVertices = planePly.pVertices;
	planeMesh->indexCount = planePly.indexCount;
	planeMesh->pIndices = planePly.pIndices;
	ezecsSetFggMaterial(scene, &planeMat, plane);
	planeTransform->position[1] = -4.0f;
	planeTransform->scale[0] = 1.0f;
	planeTransform->scale[1] = 1.0f;
	planeTransform->scale[2] = 1.0f;

#endif // RANDOM

#ifdef LORENZ
	//graph 
	uint32_t graph = ezecsCreateEntity();
	FggMesh* graphMesh = ezecsAddFggMesh(scene, graph);
	graphMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	graphMesh->vertexCount = 5000 * 3;
	lorenzAttractor(10.0f, 28.0f, 2.66f, 0.01f, graphMesh);
	ezecsSetFggMaterial(scene, &lorenzMaterial, graph);
	FggTransform* graphTransform = ezecsAddFggTransform(scene, graph);
	graphTransform->rotation[1] = 180.0f;
	graphTransform->scale[0] = 1.0f;
	graphTransform->scale[1] = 1.0f;
	graphTransform->scale[2] = 1.0f;
#endif // LORENZ

#ifdef SERVOS
	PlyFileData servoply = { 0 };
	plyLoadFile("../Assets/Meshes/Servo/servoMotor.ply", &servoply, 0);
	FggMesh servoMesh = {
		servoply.vertexCount * servoply.vertexStride,
		servoply.pVertices,
		servoply.indexCount,
		servoply.pIndices,
		FGG_MESH_SETUP_STATIC_MESH,
		0, 0, 0, 0
	};
	PlyFileData crossply = { 0 };
	plyLoadFile("../Assets/Meshes/Servo/servoMotorCross.ply", &crossply, 0);
	FggMesh crossMesh = {
		crossply.vertexCount * crossply.vertexStride,
		crossply.pVertices,
		crossply.indexCount,
		crossply.pIndices,
		FGG_MESH_SETUP_STATIC_MESH,
		0, 0, 0, 0
	};

	//servo0
	uint32_t servo0 = ezecsCreateEntity();
	ezecsSetFggMesh(scene, &servoMesh, servo0);
	FggTransform* servo0Transform = ezecsAddFggTransform(scene, servo0);
	servo0Transform->scale[0] = 1.0f;
	servo0Transform->scale[1] = 1.0f;
	servo0Transform->scale[2] = 1.0f;
	ezecsSetFggMaterial(scene, &servo0Mat, servo0);

	//cross0
	uint32_t cross0 = ezecsCreateEntity();
	FggMesh* cross0Mesh = ezecsAddFggMesh(scene, cross0);
	cross0Mesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	cross0Mesh->vertexCount = crossply.vertexCount * crossply.vertexStride;
	cross0Mesh->pVertices = crossply.pVertices;
	cross0Mesh->indexCount = crossply.indexCount;
	cross0Mesh->pIndices = crossply.pIndices;
	FggTransform* cross0Transform = ezecsAddFggTransform(scene, cross0);
	cross0Transform->position[0] = 0.316788f;
	cross0Transform->position[1] = -1.65641f;
	cross0Transform->position[2] = 0.013092f;
	cross0Transform->scale[0] = 1.0f;
	cross0Transform->scale[1] = 1.0f;
	cross0Transform->scale[2] = 1.0f;
	ezecsSetFggMaterial(scene, &cross0Mat, cross0);

	//servo1
	//uint32_t servo1 = ezecsCreateEntity();
	//ezecsSetFggMesh(scene, &servoMesh, servo1);
	//FggTransform* servo1Transform = ezecsAddFggTransform(scene, servo1);
	//servo1Transform->position[0] = -0.321191;
	//servo1Transform->position[1] = 2.04666;
	//servo1Transform->position[2] = -0.5176;
	//servo1Transform->scale[0] = 1.0f;
	//servo1Transform->scale[1] = 1.0f;
	//servo1Transform->scale[2] = 1.0f;
	//ezecsSetFggMaterial(scene, &servo1Mat, servo1);

	uint32_t transSizes[1] = { 12 };
	uint32_t transStrides[1] = { 64 + 12 };
	uint32_t transbinStride[1] = { 12 };

#endif // SERVOS

	fggSceneInit(core, scene);
	fggInitCommands(&core);

	while (fggIsWindowActive(core.window.window)) {

		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
		fggGetCursorPosition(core.window, &core.window.cursorPosX, &core.window.cursorPosY);

		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
		
#ifdef RANDOM
		handTransform->rotation[1] += 50.0f * time.deltaTime;
		lucyTransform->rotation[1] += 25.0f * time.deltaTime;
		textTransform->rotation[1] -= 100 * time.deltaTime;
#endif // RANDOM

#ifdef SERVOS
#endif // SERVOS
		fggSceneUpdate(core, time, scene);
	
		fggFrameEnd(core, imageIndex);
	}

	fggDestroyPipeline(core, &meshMaterial.pipelineData);
	fggDestroyPipeline(core, &wireframeMaterial.pipelineData);
	fggDestroyPipeline(core, &lineMaterial.pipelineData);

	fggSceneRelease(core, scene);

#ifdef RANDOM
	plyFree(&handply);
	plyFree(&lucyply);
	plyFree(&planePly);
#endif // RANDOM

#ifdef SERVOS
	plyFree(&servoply);
#endif // SERVOS

	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}