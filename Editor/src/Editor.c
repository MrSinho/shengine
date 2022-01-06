#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <engine/shEngine.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkDrawLoop.h>

int main() {

	ShEngine engine = { 0 };
	shWindowSetup("SH-Engine Editor", 720, 480, &engine.window);
	engine.core = shVkCoreInitPrerequisites();
	shCreateInstance(&engine.core, "SH-Engine editor", "SH-Engine", engine.window.instance_extension_count, engine.window.pp_instance_extensions);
	shCreateWindowSurface(&engine);
	shSetPhysicalDevice(&engine.core);
	shSetLogicalDevice(&engine.core);
	shGetGraphicsQueue(&engine.core);
	shGetComputeQueue(&engine.core);
	shInitSwapchainData(&engine.core);
	shInitDepthData(&engine.core);
	shCreateRenderPass(&engine.core);
	shSetFramebuffers(&engine.core);
	shSetSyncObjects(&engine.core);
	shInitCommands(&engine.core);

	ShFd materials_descriptor = { "../assets/SceneDescriptors/materials.json" };
	ShFd scene_descriptor = { "../assets/SceneDescriptors/scene.json" };
	ShFd physics_descriptor = { "../assets/SceneDescriptors/physics.json" };
	shInitDescriptor(&materials_descriptor);
	shInitDescriptor(&scene_descriptor);
	shInitDescriptor(&physics_descriptor);

	uint32_t material_count = 0;
	ShMaterial* p_materials = NULL;
	shLoadMaterials(materials_descriptor.path, &material_count, &p_materials);

	shCreateScene(&engine.scenes[0]);
	shLoadScene(scene_descriptor.path, p_materials, &engine.scenes[0]);

	shLoadPhysicsWorld(physics_descriptor.path, &engine.physics_host);

	shSceneInit(&engine, 0);
	
	while (shIsWindowActive(engine.window.window)) {

		shPollEvents();
		shGetTime(&engine.time);
		shFrameReset(engine.core);
		shGetCursorPosition(engine.window, &engine.window.cursor_pos_x, &engine.window.cursor_pos_y);
		if (shListenFd(&materials_descriptor)) {
			shReloadMaterials(materials_descriptor, &material_count, &p_materials);
			shReloadScene(&engine, 0, scene_descriptor, p_materials);
			shReloadPhysicsWorld(physics_descriptor, &engine.scenes[0], &engine.physics_host);
			shSetTime(0.0, &engine.time);
		}
		if (shListenFd(&scene_descriptor) || shListenFd(&physics_descriptor)) {
			shReloadScene(&engine, 0, scene_descriptor, p_materials);
			shReloadPhysicsWorld(physics_descriptor, &engine.scenes[0], &engine.physics_host);
			shSetTime(0.0, &engine.time);
		}

		uint32_t image_index = 0;
		shFrameBegin(engine.core, &image_index);
		
		shSceneUpdate(&engine, 0);

		shFrameEnd(engine.core, image_index);
	}

	shMaterialsRelease(&material_count, &p_materials);
	shSceneRelease(&engine, 0);

	shVulkanRelease(&engine.core);

	return 0;
}
