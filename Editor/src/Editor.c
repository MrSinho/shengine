#include <FGG_API.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void updateBehaviour(const FggTime time, FggScene* scene) {
	for (uint32_t entity = 0; entity < scene->entity_count; entity++) {
		if (fggHasFggIdentity(scene, entity)) {
			FggIdentity* identity = fggGetFggIdentity(scene, entity);
			if (strcmp(identity->name, "rotator") == 0) {
				// I already know it has a transform component
				FggTransform* t = fggGetFggTransform(scene, entity);
				t->rotation[1] += 10.0f * time.delta_time;
			}
		}
	}
}

void fggReloadMaterialInfo(const FggDescriptorHandle mat_info_descriptor, uint32_t* p_mat_info_count, FggMaterialInfo** pp_mat_infos) {
	free(*pp_mat_infos); *p_mat_info_count = 0;
	fggLoadMaterialInfos(mat_info_descriptor.path, p_mat_info_count, pp_mat_infos);
}

void fggReloadScene(const FggVkCore core, const FggDescriptorHandle scene_descriptor, FggMaterialInfo* p_mat_infos, FggScene* p_scene) {
		fggSceneRelease(core, p_scene);
		fggLoadScene(scene_descriptor.path, p_mat_infos, p_scene);
		fggSceneInit(core, p_scene);
}


int main() {

	FggTime time = { 0 };
	FggVkCore core = fggVkCoreInitPrerequisites(720, 480, "Foggy-Engine Editor");

	fggInitVulkan(&core);
	fggInitSwapchainData(&core);

	fggCreateRenderPass(&core);
	fggSetFramebuffers(&core);
	fggSetSyncObjects(&core);
	fggInitCommands(&core);

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
			fggReloadMaterialInfo(mat_info_descriptor, &mat_info_count, &p_mat_infos);
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

	free(p_mat_infos);
	fggSceneRelease(core, &scene);
	

	fggSurfaceRelease(&core);
	fggCmdRelease(&core);
	fggCoreRelease(&core);

	return 0;
}