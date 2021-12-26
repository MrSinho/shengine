#include <SH_API.h>
#include <shLinearAlgebraTypes.h>
#include <shPhysics.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void updateBehaviour(const ShTime time, ShRigidBody** pp_rbodies, uint32_t rbody_count, ShScene* scene) {
	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		if (shHasShIdentity(scene, entity)) {
			ShIdentity* identity = shGetShIdentity(scene, entity);
			if (strcmp(identity->name, "rotator") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				t->rotation[1] -= 150.0f * (float)time.delta_time;
			}
		}
	}
}

int main() {

	ShVkCore core = shVkCoreInitPrerequisites(1900/2, 1000, "SH-Engine Editor");

	shInitVulkan(&core);

	ShDescriptorHandle mat_info_descriptor = { "../Assets/SceneDescriptors/materials.json" };
	ShDescriptorHandle scene_descriptor = { "../Assets/SceneDescriptors/scene.json" };
	ShDescriptorHandle physics_descriptor = { "../Assets/SceneDescriptors/physics.json" };
	shInitDescriptor(&mat_info_descriptor);
	shInitDescriptor(&scene_descriptor);
	shInitDescriptor(&physics_descriptor);

	uint32_t mat_info_count = 0;
	ShMaterialInfo* p_mat_infos = NULL;
	shLoadMaterialInfos(mat_info_descriptor.path, &mat_info_count, &p_mat_infos);

	ShScene scene = { 0 };
	shCreateScene(&scene);
	shLoadScene(scene_descriptor.path, p_mat_infos, &scene);

	ShDynamicsWorld dynamics = { 0 };
	shLoadPhysicsWorld(physics_descriptor.path, &scene, &dynamics);

	shSceneInit(core, &scene);

	ShTime time = { 0 };
	while (shIsWindowActive(core.window.window)) {

		shPollEvents();
		shGetTime(&time);
		shFrameReset(core);
		shGetCursorPosition(core.window, &core.window.cursor_pos_x, &core.window.cursor_pos_y);
		if (shListenDescriptor(&mat_info_descriptor)) {
			shReloadMaterialInfos(mat_info_descriptor, &mat_info_count, &p_mat_infos);
			shReloadScene(core, scene_descriptor, p_mat_infos, &scene);
			shSetTime(0.0, &time);
		}
		if (shListenDescriptor(&scene_descriptor) || shListenDescriptor(&physics_descriptor)) {
			shReloadScene(core, scene_descriptor, p_mat_infos, &scene);
			shReloadPhysicsWorld(physics_descriptor, &scene, &dynamics);
			shSetTime(0.0, &time);
		}

		uint32_t image_index = 0;
		shFrameBegin(core, &image_index);
		
		shSceneUpdate(core, time, &scene);
		shDynamicsWorldSimulate((shreal)time.delta_time, &dynamics);
		updateBehaviour(time, dynamics.pp_rbodies, dynamics.rbody_count, &scene);

		shFrameEnd(core, image_index);
	}

	shMaterialInfosRelease(&mat_info_count, &p_mat_infos);
	shSceneRelease(core, &scene);

	shVulkanRelease(&core);

	return 0;
}
