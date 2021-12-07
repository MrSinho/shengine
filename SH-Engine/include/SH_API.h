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

#include "shWindow.h"
#include "shInput.h"
#include "shTime.h"
#include "shUtilities.h"

#include "shDescriptorHandle.h"



static void shReloadMaterialInfos(const ShDescriptorHandle mat_info_descriptor, uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos) {
	shMaterialInfosRelease(p_mat_info_count, pp_mat_infos);
	shLoadMaterialInfos(mat_info_descriptor.path, p_mat_info_count, pp_mat_infos);
}

static void shReloadScene(const ShVkCore core, const ShDescriptorHandle scene_descriptor, ShMaterialInfo* p_mat_infos, ShScene* p_scene) {
	shSceneRelease(core, p_scene);
	shLoadScene(scene_descriptor.path, p_mat_infos, p_scene);
	shSceneInit(core, p_scene);
}

#endif // SH_API_H