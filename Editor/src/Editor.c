#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>

void fggSetupMaterial(const FggVkCore core, const FggVkFixedStates fixedStates, void** ppPushConstants, FggMaterial* pMaterial) {
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

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(1280, 720, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);

	FggVkFixedStates fixedStates = { 0 };
	FggFixedStateFlags fixedStateFlags = FGG_FIXED_STATES_WIREFRAME_BIT | 
										 FGG_FIXED_STATES_VERTEX_POSITIONS_BIT |
										 FGG_FIXED_STATES_VERTEX_NORMALS_BIT | 
										 FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
	fggSetFixedStates(core, fixedStateFlags, &fixedStates);

							//projection		//view
	mat4 pConst[2] = { GLM_MAT4_IDENTITY_INIT, GLM_MAT4_IDENTITY_INIT };
	FggMaterial baseMaterial = { 0 };
	fggSetupMaterial(core, fixedStates, (void**)pConst, &baseMaterial);

	ezecsScene scene = { 0 };
	ezecsCreateScene(scene);


	PlyFileData geometryply = { 0 };
	plyLoadFile("../Assets/Meshes/prism.ply", &geometryply, 0);
	
	uint32_t geometry = ezecsCreateEntity();
	FggTransform* geometryTransform = ezecsAddFggTransform(scene, geometry);
	FggMesh* geometryMesh = ezecsAddFggMesh(scene, geometry);
	geometryMesh->flags = FGG_MESH_SETUP_STATIC_MESH;
	geometryMesh->vertexCount = geometryply.vertexCount * geometryply.vertexStride;
	geometryMesh->pVertices = geometryply.pVertices;
	geometryMesh->indexCount = geometryply.indexCount;
	geometryMesh->pIndices = geometryply.pIndices;
	//float vertices[24] = {
	//	0.0f, -1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
	//	1.0f,  1.0f, 0.0f,  0.0f, 0.0f, 0.0f,
	//	-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	//};
	//geometryMesh->pVertices = calloc(24, sizeof(uint32_t));
	//if (geometryMesh->pVertices == NULL) { return EXIT_FAILURE; }
	//for (uint32_t i = 0; i < 24; i++) {
	//	geometryMesh->pVertices[i] = vertices[i];
	//}
	//geometryMesh->vertexCount = 24;
	ezecsSetFggMaterial(scene, &baseMaterial, geometry);
	geometryTransform->scale[0] = 1.0f;
	geometryTransform->scale[1] = 1.0f;
	geometryTransform->scale[2] = 1.0f;
	geometryTransform->position[0] = 1.0f;
	geometryTransform->position[2] = -3.0f;
	geometryTransform->rotation[1] = -100.0f;

	fggSceneInit(core, fixedStates, scene);
	fggInitCommands(&core);

	while (fggIsWindowActive(core.window.window)) {
		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
	
		uint32_t imageIndex = 0;
		fggFrameBegin(core, &imageIndex);
	
		fggSetProjection(core.window, pConst[0]);

		fggSetView(pConst[1]);

		geometryMesh->pVertices[0] = (float)sin((float)time.now);
		fggSceneUpdate(core, fixedStates, scene);
	
		fggFrameEnd(core, imageIndex);
	}
	
	plyFree(&geometryply);
	fggSceneRelease(core, scene);
	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}