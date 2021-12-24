#include <SH_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ShPhysics.h>

void updateBehaviour(const ShTime time, ShRigidBody** pp_rbodies, uint32_t rbody_count, ShScene* scene) {
	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		if (shHasShIdentity(scene, entity)) {
			ShIdentity* identity = shGetShIdentity(scene, entity);
			if (strcmp(identity->name, "rotator") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				t->rotation[1] -= 150.0f * time.delta_time;
			}
			else if (strcmp(identity->name, "earth") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				memcpy(t->position, pp_rbodies[0]->position, sizeof(shvec3));
			}
			else if (strcmp(identity->name, "moon") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				memcpy(t->position, pp_rbodies[1]->position, sizeof(shvec3));
			}
		}
	}
}

int main() {

	ShTime time = { 0 };
	ShVkCore core = shVkCoreInitPrerequisites(720, 480, "SH-Engine Editor");

	shInitVulkan(&core);

	ShDescriptorHandle mat_info_descriptor = { "../Assets/SceneDescriptors/materials.json" };
	ShDescriptorHandle scene_descriptor = { "../Assets/SceneDescriptors/scene.json" };
	shInitDescriptor(&mat_info_descriptor);
	shInitDescriptor(&scene_descriptor);

	uint32_t mat_info_count = 0;
	ShMaterialInfo* p_mat_infos = NULL;
	shLoadMaterialInfos(mat_info_descriptor.path, &mat_info_count, &p_mat_infos);

	ShScene scene = { 0 };
	shCreateScene(&scene);
	shLoadScene(scene_descriptor.path, p_mat_infos, &scene);
	
	shSceneInit(core, &scene);

	ShRigidBody earth = { 0 };
	earth.mass = 597.0f;
	shvec3 pos0 = { 0.0f, 0.0f, 0.0f };
	shDynamicsSetPosition(pos0, &earth);

	ShRigidBody moon = { 0 };
	moon.mass = 7.34f;
	shvec3 pos1 = { 10.0f, 0.0f, 0.0f };
	shDynamicsSetPosition(pos1, &moon);

	ShRigidBody* pp_rbodies[2] = {
		&earth, &moon
	};

	while (shIsWindowActive(core.window.window)) {

		shPollEvents();
		shGetTime(&time);
		shFrameReset(core);
		shGetCursorPosition(core.window, &core.window.cursor_pos_x, &core.window.cursor_pos_y);
		if (shListenDescriptor(&mat_info_descriptor)) {
			shReloadMaterialInfos(mat_info_descriptor, &mat_info_count, &p_mat_infos);
			shReloadScene(core, scene_descriptor, p_mat_infos, &scene);
		}
		if (shListenDescriptor(&scene_descriptor)) {
			shReloadScene(core, scene_descriptor, p_mat_infos, &scene);
			shDynamicsSetPosition(pos0, &earth);
			shDynamicsSetPosition(pos1, &moon);
		}

		uint32_t image_index = 0;
		shFrameBegin(core, &image_index);
		
		shSceneUpdate(core, time, &scene);
		shDynamicsWorldSimulate(SH_DYNAMICS_WORLD_GRAVITY, time.delta_time * DEC(1.0E3), pp_rbodies, 2);
		updateBehaviour(time, pp_rbodies, 2, &scene);

		shFrameEnd(core, image_index);
	}

	shMaterialInfosRelease(&mat_info_count, &p_mat_infos);
	shSceneRelease(core, &scene);

	shVulkanRelease(&core);

	return 0;
}