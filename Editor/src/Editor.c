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
				t->rotation[1] -= 150.0f * time.delta_time;
			}
			else if (strcmp(identity->name, "earth") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				memcpy(t->position, pp_rbodies[0]->position, sizeof(shvec3));
				//printf("earth pos: %f\n", t->position[0]);
			}
			else if (strcmp(identity->name, "moon") == 0) {
				ShTransform* t = shGetShTransform(scene, entity);
				memcpy(t->position, pp_rbodies[1]->position, sizeof(shvec3));
				//printf("moon pos: %f\n", t->position[0]);
			}
		}
	}
}

int main() {

	ShVkCore core = shVkCoreInitPrerequisites(720, 480, "SH-Engine Editor");

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

	//shvec3 earth_pos = SH_VEC3_INIT;
	//shvec3 moon_pos = { DEC(9.0), DEC(0.0), DEC(0.0) };
	//
	//ShRigidBody earth = { 0 };
	//earth.mass = DEC(600.0);
	//earth.position = earth_pos;
	//shDynamicsSetCollisionShape(SH_COLLISION_SHAPE_SPHERE, &earth);
	//shDynamicsSetCollisionSphereRadius(DEC(7.0), &earth);
	//
	//ShRigidBody moon = { 0 };
	//moon.mass = DEC(8.0);
	//moon.position = moon_pos;
	//shDynamicsSetCollisionShape(SH_COLLISION_SHAPE_SPHERE, &moon);
	//shDynamicsSetCollisionSphereRadius(DEC(1.0), &moon);
	//
	//ShRigidBody* pp_rbodies[2] = {
	//	&earth, &moon
	//};
	//ShDynamicsWorld dynamics = {
	//	SH_DYNAMICS_WORLD_GRAVITY | SH_DYNAMICS_WORLD_NEWTON_3RD_LAW,
	//	pp_rbodies,
	//	2
	//};
	/*
	dt -1.978497
moon 34.810707
*/
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
		shDynamicsWorldSimulate(time.delta_time, &dynamics);
		updateBehaviour(time, dynamics.pp_rbodies, dynamics.rbody_count, &scene);

		shFrameEnd(core, image_index);
	}

	shMaterialInfosRelease(&mat_info_count, &p_mat_infos);
	shSceneRelease(core, &scene);

	shVulkanRelease(&core);

	return 0;
}
