#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <engine/shEngine.h>

#include <shfd/shFd.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkDrawLoop.h>

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

	ShFd materials_descriptor = { "../assets/SceneDescriptors/materials.json" };
	ShFd scene_descriptor = { "../assets/SceneDescriptors/scene.json" };
	ShFd physics_descriptor = { "../assets/SceneDescriptors/physics.json" };
	shInitDescriptor(&materials_descriptor);
	shInitDescriptor(&scene_descriptor);
	shInitDescriptor(&physics_descriptor);

	shLoadMaterials(&engine.core, materials_descriptor.path, &engine.material_count, &engine.p_materials);

	shCreateScene(&engine.scenes[0]);
	shLoadScene(scene_descriptor.path, &engine.p_materials, &engine.scenes[0]);

	shLoadPhysicsWorld(physics_descriptor.path, &engine.physics_host);

	shSceneInit(&engine, 0);
	
	while (shIsWindowActive(engine.window.window)) {

		shPollEvents();
		shGetTime(&engine.time);
		shFrameReset(&engine.core);
		shGetCursorPosition(&engine.window);
		//if (shListenFd(&materials_descriptor)) {
		//	shReloadMaterials(&engine.core, materials_descriptor, &material_count, &p_materials);
		//	shReloadScene(&engine, 0, scene_descriptor, p_materials);
		//	shReloadPhysicsWorld(physics_descriptor, &engine.scenes[0], &engine.physics_host);
		//	shSetTime(0.0, &engine.time);
		//}
		//if (shListenFd(&scene_descriptor) || shListenFd(&physics_descriptor)) {
		//	shReloadScene(&engine, 0, scene_descriptor, p_materials);
		//	shReloadPhysicsWorld(physics_descriptor, &engine.scenes[0], &engine.physics_host);
		//	shSetTime(0.0, &engine.time);
		//}

		uint32_t image_index = 0;
		shFrameBegin(&engine.core, &image_index);
		
		shSceneUpdate(&engine, 0);

		shFrameEnd(&engine.core, image_index);
	}

	shMaterialsRelease(&engine.core, &engine.material_count, &engine.p_materials);
	shSceneRelease(&engine, 0);

	shVulkanRelease(&engine.core);

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
