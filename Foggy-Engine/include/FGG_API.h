#ifndef FGG_API_H
#define FGG_API_H

#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggVkMemoryInfo.h"
#include "fggDrawLoop.h"
#include "fggProjection.h"
#include "fggView.h"

#include "fggSceneHandle.h"
#include "fggComponentEmbedded.h"
#include "fggComponents.h"

#include "PlyImporter.h"

#include "fggWindow.h"
#include "fggInput.h"
#include "fggTime.h"
#include "fggUtilities.h"

#include "fggDescriptorHandle.h"



static void fggReloadMaterialInfos(const FggDescriptorHandle mat_info_descriptor, uint32_t* p_mat_info_count, FggMaterialInfo** pp_mat_infos) {
	fggMaterialInfosRelease(p_mat_info_count, pp_mat_infos);
	fggLoadMaterialInfos(mat_info_descriptor.path, p_mat_info_count, pp_mat_infos);
}

static void fggReloadScene(const FggVkCore core, const FggDescriptorHandle scene_descriptor, FggMaterialInfo* p_mat_infos, FggScene* p_scene) {
	fggSceneRelease(core, p_scene);
	fggLoadScene(scene_descriptor.path, p_mat_infos, p_scene);
	fggSceneInit(core, p_scene);
}

#endif // FGG_API_H