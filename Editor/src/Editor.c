#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#include "fggSceneManager.h"
//#define SERVOS
#define RANDOM
#define LORENZ

#ifdef RANDOM

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

void lorenzAttractor(float a, float b, float c, float dTime, FggMeshInfo* mesh) {
	if (mesh->p_vertices != NULL) {
		free(mesh->p_vertices);
	}
	if (mesh->p_indices != NULL) {
		free(mesh->p_indices); 
	}
	mesh->p_vertices = calloc(mesh->vertex_count, sizeof(float));
	if (mesh->p_vertices == NULL) { return; }
	for (uint32_t i = 0; i < mesh->vertex_count; i += 3) {
		float* hvertex;
		float vertex[3];
		if (i == 0) {
			hvertex = lorenzAttractorVertex(10.0f, 28.0f, 2.66f, dTime, 0.01f, 0.0f, 0.0f);
		}
		else {
			hvertex = lorenzAttractorVertex(10.0f, 28.0f, 2.66f, dTime, mesh->p_vertices[i - 3], mesh->p_vertices[i - 2], mesh->p_vertices[i - 1]);
		}
		memcpy(vertex, hvertex, sizeof(float) * 3);
		free(hvertex);
		mesh->p_vertices[i]		= vertex[0];
		mesh->p_vertices[i + 1]	= vertex[1];
		mesh->p_vertices[i + 2]	= vertex[2];
	}
	fggGenerateGraphIndices(mesh);
}

#endif // RANDOM

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(720, 480, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);
	fggInitCommands(&core);

	//MATERIAL INFOS
	FggMaterialInfo wireframeMaterialInfo = {
		"../Shaders/bin/Mesh.vert.spv", 
		"../Shaders/bin/Mesh.frag.spv",
		sizeof(mat4), 
		VK_SHADER_STAGE_VERTEX_BIT,
		sizeof(mat4) * 2, 
		VK_SHADER_STAGE_VERTEX_BIT,
		FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
			FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
			FGG_FIXED_STATES_VERTEX_NORMALS_BIT |
			FGG_FIXED_STATES_VERTEX_TCOORDS_BIT
	};

	FggMaterialInfo lineMaterialInfo = {
		"../Shaders/bin/Line.vert.spv",
		"../Shaders/bin/Line.frag.spv",
		sizeof(mat4),
		VK_SHADER_STAGE_VERTEX_BIT,
		sizeof(mat4) * 2,
		VK_SHADER_STAGE_VERTEX_BIT,
		FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT |
			FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST |
			FGG_FIXED_STATES_VERTEX_POSITIONS_BIT
	};

#ifndef NDEBUG
	fggCompileGLSLShader("../Shaders/src/Mesh.vert", "../Shaders/bin/Mesh.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Mesh.frag", "../Shaders/bin/Mesh.frag.spv");
	fggCompileGLSLShader("../Shaders/src/Line.vert", "../Shaders/bin/Line.vert.spv");
	fggCompileGLSLShader("../Shaders/src/Line.frag", "../Shaders/bin/Line.frag.spv");
#endif // NDEBUG

	//MATERIALS
	FggMaterial wireframeMaterial, lineMaterial = { 0 };
	fggSetupMaterial(core, wireframeMaterialInfo, &wireframeMaterial);
	fggSetupMaterial(core, lineMaterialInfo, &lineMaterial);

	FggScene scene = { 0 };
	fggCreateScene(&scene);

	//camera
	uint32_t camEntity = fggCreateEntity(&scene);
	FggCamera cam = {
		45.0f,								//fov;
		0.1f,								//nc;
		150.0f,								//fc;
		{0.0f},								//projection;
		{0.0f},								//view;
		FGG_CAMERA_SETUP_FREE_FLIGHT_BIT,	//flags
	};
	fggSetFggCamera(&scene, &cam, camEntity);
	FggTransform* camTransform = fggAddFggTransform(&scene, camEntity);
	camTransform->position[2] = 2.0f;

#ifdef RANDOM
	//hand
	PlyFileData handply = { 0 };
	plyLoadFile("../Assets/Meshes/stanfordHand.ply", &handply, 0);
	uint32_t hand = fggCreateEntity(&scene);
	FggTransform* handTransform = fggAddFggTransform(&scene, hand);
	FggMeshInfo* handMesh = fggAddFggMeshInfo(&scene, hand);
	handMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	handMesh->vertex_count = handply.vertexCount * handply.vertexStride;
	handMesh->p_vertices = handply.pVertices;
	handMesh->index_count = handply.indexCount;
	handMesh->p_indices = handply.pIndices;
	fggCreateMaterialInstance(core, &wireframeMaterial, fggAddFggMaterial(&scene, hand));
	handTransform->scale[0] = 0.5f;
	handTransform->scale[1] = 0.5f;
	handTransform->scale[2] = 0.5f;
	handTransform->position[0] = -3.5f;
	handTransform->position[1] = -1.85f;
	handTransform->position[2] = -6.0f;
	
	//lucy
	//PlyFileData lucyply = { 0 };
	//plyLoadFile("../Assets/Meshes/stanfordLucy.ply", &lucyply, 0);
	//uint32_t lucy = fggCreateEntity();
	//FggTransform* lucyTransform = fggAddFggTransform(scene, lucy);
	//FggMeshInfo* lucyMesh = fggAddFggMeshInfo(scene, lucy);
	//lucyMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	//lucyMesh->vertex_count = lucyply.vertexCount * lucyply.vertexStride;
	//lucyMesh->p_vertices = lucyply.pVertices;
	//lucyMesh->index_count = lucyply.indexCount;
	//lucyMesh->p_indices = lucyply.pIndices;
	//fggCreateMaterialInstance(core, &wireframeMaterial, fggAddFggMaterial(scene, lucy));
	//lucyTransform->scale[0] = 1.0f;
	//lucyTransform->scale[1] = 1.0f;
	//lucyTransform->scale[2] = 1.0f;
	//lucyTransform->position[0] = 2.3f;
	//lucyTransform->position[1] = 1.0f;
	//lucyTransform->position[2] = -2.0f;
	//
	////text
	//PlyFileData textply = { 0 };
	//plyLoadFile("../Assets/Meshes/text.ply", &textply, 0);
	//uint32_t text = fggCreateEntity();
	//FggTransform* textTransform = fggAddFggTransform(scene, text);
	//FggMeshInfo* textMesh = fggAddFggMeshInfo(scene, text);
	//textMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	//textMesh->vertex_count = textply.vertexCount * textply.vertexStride;
	//textMesh->p_vertices = textply.pVertices;
	//textMesh->index_count = textply.indexCount;
	//textMesh->p_indices = textply.pIndices;
	//fggCreateMaterialInstance(core, &wireframeMaterial, fggAddFggMaterial(scene, text));
	//textTransform->scale[0] = 1.0f;
	//textTransform->scale[1] = 1.0f;
	//textTransform->scale[2] = 1.0f;
	//
	////graph 
	//uint32_t graph = fggCreateEntity();
	//FggMeshInfo* graphMesh = fggAddFggMeshInfo(scene, graph);
	//graphMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	//graphMesh->vertex_count = 5000 * 3;
	//lorenzAttractor(10.0f, 28.0f, 2.66f, 0.01f, graphMesh);
	//fggCreateMaterialInstance(core, &lineMaterial, fggAddFggMaterial(scene, graph));
	//FggTransform* graphTransform = fggAddFggTransform(scene, graph);
	//graphTransform->rotation[1] = 180.0f;
	//graphTransform->scale[0] = 1.0f;
	//graphTransform->scale[1] = 1.0f;
	//graphTransform->scale[2] = 1.0f;
#endif // RANDOM

	fggSceneInit(core, &scene);
	FggSceneDescriptorHandle scene_descriptor = { "../Assets/SceneDescriptors/scene0.json"};
	fggInitSceneDescriptor(&scene_descriptor);

	while (fggIsWindowActive(core.window.window)) {

		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
		fggGetCursorPosition(core.window, &core.window.cursor_pos_x, &core.window.cursor_pos_y);
		fggListenSceneDescriptor(&scene_descriptor, &scene);

		uint32_t image_index = 0;
		fggFrameBegin(core, &image_index);
		
#ifdef RANDOM
		handTransform->rotation[1] += 50.0f * time.delta_time;
		//lucyTransform->rotation[1] += 25.0f * time.delta_time;
		//textTransform->rotation[1] -= 100 * time.delta_time;
#endif // RANDOM

#ifdef SERVOS
#endif // SERVOS
		fggSceneUpdate(core, time, &scene);
	
		fggFrameEnd(core, image_index);
	}

	fggDestroyPipeline(core, &wireframeMaterial.pipeline_data);
	fggDestroyPipeline(core, &lineMaterial.pipeline_data);

	fggSceneRelease(core, &scene);

#ifdef RANDOM
	plyFree(&handply);
	//plyFree(&lucyply);
#endif // RANDOM

#ifdef SERVOS
	plyFree(&servoply);
#endif // SERVOS

	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}