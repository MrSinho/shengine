#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	FggSceneDescriptorHandle scene_descriptor = { "../Assets/SceneDescriptors/scene0.json" };
	fggInitSceneDescriptor(&scene_descriptor);
	fggLoadScene(scene_descriptor.path, &scene);
	
	fggSceneInit(core, &scene);

	while (fggIsWindowActive(core.window.window)) {

		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
		fggGetCursorPosition(core.window, &core.window.cursor_pos_x, &core.window.cursor_pos_y);
		if (fggListenSceneDescriptor(&scene_descriptor, &scene)) {
			fggSceneRelease(core, &scene);
			fggLoadScene(scene_descriptor.path, &scene);
			fggSceneInit(core, &scene);
		}

		uint32_t image_index = 0;
		fggFrameBegin(core, &image_index);
		
		fggSceneUpdate(core, time, &scene);
	
		fggFrameEnd(core, image_index);
	}

	fggDestroyPipeline(core, &wireframeMaterial.pipeline_data);
	fggDestroyPipeline(core, &lineMaterial.pipeline_data);

	fggSceneRelease(core, &scene);
	

	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}




























#ifdef RANDOM

float* lorenzAttractorVertex(float a, float b, float c, float dTime, float x, float y, float z) {

	float* vertex = calloc(3, sizeof(float));
	if (vertex == NULL) { return NULL; }
	float dx, dy, dz;

	dx = (a * (y - x)) * dTime;
	dy = (x * (b - z) - y) * dTime;
	dz = (x * y - c * z) * dTime;
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
		mesh->p_vertices[i] = vertex[0];
		mesh->p_vertices[i + 1] = vertex[1];
		mesh->p_vertices[i + 2] = vertex[2];
	}
	fggGenerateGraphIndices(mesh);
}

#endif // RANDOM