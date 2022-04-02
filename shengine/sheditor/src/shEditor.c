#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <shsharedhost/shSharedHost.h>
#include <shfd/shFd.h>

#include <shengine/shEngine.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkDrawLoop.h>

#include <shecs/shTransform.h>

#ifndef SH_EDITOR_ASSETS_PATH //defined with CMake
#define SH_EDITOR_ASSETS_PATH "../assets"
#endif//SH_ENGINE_ASSETS_PATH

#include <string.h>

#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER

char* shMakeAssetsPath(const char* src_path, char* dst_path) {
	strcpy(dst_path, SH_EDITOR_ASSETS_PATH);
	strcat(dst_path, src_path);
	return dst_path;
}

int main() {

	ShEngine engine = { 0 };
	shWindowSetup("SH-Engine Editor", 720, 480, &engine.window);
	shCreateInstance(&engine.core, "SH-Engine editor", "SH-Engine", 1, engine.window.instance_extension_count, engine.window.pp_instance_extensions);
	shCreateWindowSurface(&engine);
	shSelectPhysicalDevice(&engine.core, VK_QUEUE_GRAPHICS_BIT);
	shSetLogicalDevice(&engine.core);
	shGetGraphicsQueue(&engine.core);
	shGetComputeQueue(&engine.core);
	shInitSwapchainData(&engine.core);
	shInitDepthData(&engine.core);
	shCreateRenderPass(&engine.core);
	shSetFramebuffers(&engine.core);
	shSetSyncObjects(&engine.core);
	shCreateGraphicsCommandBuffer(&engine.core);

	ShFd materials_descriptor = { 0 };
	ShFd scene_descriptor = { 0 };
	ShFd simulation_descriptor = { 0 };
	//shInitDescriptor(&materials_descriptor);
	//shInitDescriptor(&scene_descriptor);
	//shInitDescriptor(&simulation_descriptor);
	shMakeAssetsPath("/descriptors/materials.json", materials_descriptor.path);
	shMakeAssetsPath("/descriptors/scene.json", scene_descriptor.path);
	shMakeAssetsPath("/descriptors/simulation.json", simulation_descriptor.path);

	shLoadMaterials(&engine.core, materials_descriptor.path, &engine.material_count, &engine.p_materials);

	shCreateScene(&engine.scene);
	shLoadScene(scene_descriptor.path, &engine.p_materials, &engine.scene);

	shSceneInit(&engine);

	ShSimulationHandle simulation = { 0 };	
	shLoadSimulation(simulation_descriptor.path, &engine, &simulation);
	shSimulationLoadSymbols(&simulation);

	shSimulationStart(&simulation, &engine);

	while (shIsWindowActive(engine.window.window)) {
		shUpdateWindow(&engine);
		
		//if (shListenFd(&materials_descriptor)) {
		//	shReloadMaterials(&engine.core, materials_descriptor, &material_count, &p_materials);
		//	shReloadScene(&engine, 0, scene_descriptor, p_materials);
		//	shReloadPhysicsWorld(physics_descriptor, &engine.scene, &engine.physics_host);
		//	shSetTime(0.0, &engine.time);
		//}
		//if (shListenFd(&scene_descriptor) || shListenFd(&physics_descriptor)) {
		//	shReloadScene(&engine, 0, scene_descriptor, p_materials);
		//	shReloadPhysicsWorld(physics_descriptor, &engine.scene, &engine.physics_host);
		//	shSetTime(0.0, &engine.time);
		//}
		shFrameReset(&engine.core);

		uint32_t image_index = 0;
		shFrameBegin(&engine.core, &image_index);
		
		shSimulationUpdate(&simulation, &engine);

		shSceneUpdate(&engine);

		shFrameEnd(&engine.core, image_index);
	}

	shMaterialsRelease(&engine.core, &engine.material_count, &engine.p_materials);
	shSceneRelease(&engine);

	shVulkanRelease(&engine.core);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
