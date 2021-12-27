#ifndef SH_API_H
#define SH_API_H

#include "shVkCore.h"
#include "shVkPipelineData.h"
#include "shVkMemoryInfo.h"
#include "shDrawLoop.h"
#include "shProjection.h"
#include "shView.h"

#include "shSceneHandle.h"
#include "shComponentEmbedded.h"
#include "shComponents.h"

#include "PlyImporter.h"
#include "shDynamics.h"

#include "shWindow.h"
#include "shInput.h"
#include "shTime.h"
#include "shUtilities.h"

#include "shDescriptorHandle.h"

static void shInitVulkan(ShVkCore* p_core) {
	shCreateInstance(p_core);
	shCreateWindowSurface(p_core);
	shSetPhysicalDevice(p_core);
	shSetLogicalDevice(p_core);
	shGetGraphicsQueue(p_core);
	shInitSwapchainData(p_core);
	shInitDepthData(p_core);
	shCreateRenderPass(p_core);
	shSetFramebuffers(p_core);
	shSetSyncObjects(p_core);
	shInitCommands(p_core);
}

static void shReloadMaterialInfos(const ShDescriptorHandle mat_info_descriptor, uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos) {
	shMaterialInfosRelease(p_mat_info_count, pp_mat_infos);
	shLoadMaterialInfos(mat_info_descriptor.path, p_mat_info_count, pp_mat_infos);
}

static void shReloadScene(const ShVkCore core, const ShDescriptorHandle scene_descriptor, ShMaterialInfo* p_mat_infos, ShScene* p_scene) {
	shSceneRelease(core, p_scene);
	shLoadScene(scene_descriptor.path, p_mat_infos, p_scene);
	shSceneInit(core, p_scene);
}

static void shReloadPhysicsWorld(const ShDescriptorHandle physics_descriptor, ShScene* p_scene, ShDynamicsWorld* p_dynamics) {
	shDynamicsWorldRelease(p_dynamics);
	shLoadPhysicsWorld(physics_descriptor.path, p_scene, p_dynamics);
}

#endif // SH_API_H