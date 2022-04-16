#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>

#include "shecs/shEcsImplementation.h"
#include "shecs/shPhysics.h"
#include "shecs/shMaterial.h"

#include "shengine/shWindow.h"
#include "shengine/shTime.h"
#include "shengine/shInput.h"

#include "shscene/shScene.h"

#include <stdint.h>

typedef struct ShEngine {
    ShVkCore        core;
    ShWindow        window;
    ShTime          time;
    ShScene         scene;
    uint32_t        scene_count;
	ShMaterialHost*	p_materials;
	uint32_t		material_count;
    ShPhysicsHost   physics_host;
}ShEngine;

//static void shReloadMaterials(ShVkCore* p_core, const ShFd mat_info_descriptor, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials) {
//	shMaterialsRelease(p_core, p_mat_info_count, pp_materials);
//	shLoadMaterials(p_core, mat_info_descriptor.path, p_mat_info_count, pp_materials);
//}
//
//static void shReloadScene(ShEngine* p_engine, const uint32_t scene_idx, const ShFd scene_descriptor, ShMaterialHost** pp_materials) {
//	shSceneRelease(p_engine, scene_idx);
//	shLoadScene(scene_descriptor.path, pp_materials, &p_engine->scenes[scene_idx]);
//	shSceneInit(p_engine, scene_idx);
//}
//
//static void shReloadPhysicsWorld(const ShFd physics_descriptor, ShScene* p_scene, ShPhysicsHost* p_host) {
//	ShPhysicsHostRelease(p_host);
//	shLoadPhysicsWorld(physics_descriptor.path, p_host);
//}

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H