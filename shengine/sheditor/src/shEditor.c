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

#include <shgui/shgui.h>


#define SH_EDITOR_THREAD_COUNT 1



int main() {

	ShEngine engine = { 0 };
	shWindowSetup("shengine editor", 1366, 768, &engine.window);
	shCreateInstance(&engine.core, "shengine editor", "shengine", 1, engine.window.instance_extension_count, engine.window.pp_instance_extensions);
	shWindowCreateSurface(&engine);
	shSelectPhysicalDevice(&engine.core, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
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

	shMakeAssetsPath("/descriptors/materials.json", engine.materials_descriptor.path);
	shMakeAssetsPath("/descriptors/scene.json", engine.scene_descriptor.path);
	shMakeAssetsPath("/descriptors/simulation.json", engine.simulation_descriptor.path);

	shEngineManageState(&engine, shSetEngineState(&engine), 1);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
