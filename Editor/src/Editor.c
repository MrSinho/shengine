#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void updateBehaviour(const FggTime time, FggScene* scene) {
	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		if (fggHasFggIdentity(scene, entity)) {
			FggIdentity* identity = fggGetFggIdentity(scene, entity);
			if (strcmp(identity->name, "rotator") == 0) {
				FggTransform* t = fggGetFggTransform(scene, entity);
				t->rotation[1] -= 150.0f * time.delta_time;
			}
		}
	}
}

int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(720, 480, "Foggy-Engine Editor");

	fggInitVulkan(&core);

	FggDescriptorHandle mat_info_descriptor = { "../Assets/SceneDescriptors/materials.json" };
	FggDescriptorHandle scene_descriptor = { "../Assets/SceneDescriptors/scene.json" };
	fggInitDescriptor(&mat_info_descriptor);
	fggInitDescriptor(&scene_descriptor);

	uint32_t mat_info_count = 0;
	FggMaterialInfo* p_mat_infos = NULL;
	fggLoadMaterialInfos(mat_info_descriptor.path, &mat_info_count, &p_mat_infos);

	FggScene scene = { 0 };
	fggCreateScene(&scene);
	fggLoadScene(scene_descriptor.path, p_mat_infos, &scene);
	
	fggSceneInit(core, &scene);
	while (fggIsWindowActive(core.window.window)) {

		fggPollEvents();
		fggGetTime(&time);
		fggFrameReset(core);
		fggGetCursorPosition(core.window, &core.window.cursor_pos_x, &core.window.cursor_pos_y);
		if (fggListenDescriptor(&mat_info_descriptor)) {
			fggReloadMaterialInfos(mat_info_descriptor, &mat_info_count, &p_mat_infos);
			fggReloadScene(core, scene_descriptor, p_mat_infos, &scene);
		}
		if (fggListenDescriptor(&scene_descriptor)) {
			fggReloadScene(core, scene_descriptor, p_mat_infos, &scene);
		}

		uint32_t image_index = 0;
		fggFrameBegin(core, &image_index);
		
		fggSceneUpdate(core, time, &scene);
		updateBehaviour(time, &scene);

		fggFrameEnd(core, image_index);
	}

	fggMaterialInfosRelease(&mat_info_count, &p_mat_infos);
	fggSceneRelease(core, &scene);

	fggVulkanRelease(&core);

	return 0;
}