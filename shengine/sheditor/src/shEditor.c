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


#include <string.h>

#define SH_EDITOR_THREAD_COUNT 1

int main() {

	ShEngine engine = { 0 };
	shWindowSetup("shengine editor", 720, 480, &engine.window);
	shCreateInstance(&engine.core, "shengine editor", "shengine", 1, engine.window.instance_extension_count, engine.window.pp_instance_extensions);
	shCreateWindowSurface(&engine);
	shSelectPhysicalDevice(&engine.core, SH_VK_CORE_GRAPHICS | SH_VK_CORE_COMPUTE);
	shSetLogicalDevice(&engine.core);
	shGetGraphicsQueue(&engine.core);
	shGetComputeQueue(&engine.core);
	shInitSwapchainData(&engine.core);
	shInitDepthData(&engine.core);
	shCreateRenderPass(&engine.core);
	shSetFramebuffers(&engine.core);
	shCreateGraphicsCommandBuffers(&engine.core, SH_EDITOR_THREAD_COUNT);
	shCreateComputeCommandBuffers(&engine.core, SH_EDITOR_THREAD_COUNT);
	shSetSyncObjects(&engine.core);

	
	//shInitDescriptor(&materials_descriptor);
	//shInitDescriptor(&scene_descriptor);
	//shInitDescriptor(&simulation_descriptor);
	shMakeAssetsPath("/descriptors/materials.json", engine.materials_descriptor.path);
	shMakeAssetsPath("/descriptors/scene.json", engine.scene_descriptor.path);
	shMakeAssetsPath("/descriptors/simulation.json", engine.simulation_descriptor.path);

	shLoadMaterials(&engine.core, engine.materials_descriptor.path, &engine.material_count, &engine.p_materials);

	shCreateScene(&engine.scene);
	shLoadScene(engine.scene_descriptor.path, &engine.p_materials, &engine.scene);

	shSceneInit(&engine);

	//ShSimulationHandle simulation = { 0 };
	shLoadSimulation(engine.simulation_descriptor.path, &engine, &engine.simulation_host);
	shSimulationLoadSymbols(&engine.simulation_host);

	shSimulationStart(&engine);

	double input_dtime = 0.0;
	double input_last_time = 0.0;

	while (shIsWindowActive(engine.window.window)) {
		shUpdateWindow(&engine);

		input_dtime = engine.time.now - input_last_time;
		if (input_dtime >= 2.0) {
			if (shIsKeyPressed(engine.window, SH_KEY_LEFT_CONTROL) && shIsKeyPressed(engine.window, SH_KEY_R)) {
				input_last_time = engine.time.now;
				shSetTime(0.0, &engine.time);
				shResetEngineState(&engine);
				input_last_time = 0.0;
			}
		}

		shFrameReset(&engine.core, 0);

		uint32_t image_index = 0;
		shFrameBegin(&engine.core, 0, &image_index);

		shSimulationUpdate(&engine);

		shSceneUpdate(&engine);

		shFrameEnd(&engine.core, 0, image_index);
	}

	shMaterialsRelease(&engine.core, &engine.material_count, &engine.p_materials);
	shSceneRelease(&engine);
	shSimulationClose(&engine);

	shVulkanRelease(&engine.core);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
