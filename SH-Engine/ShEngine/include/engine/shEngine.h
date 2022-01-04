#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#include <shVkCore.h>
#include <stdint.h>

#include "ecs/shEcsImplementation.h"
#include "ecs/shPhysics.h"
#include "ecs/shMaterial.h"

#include "engine/shWindow.h"
#include "engine/shTime.h"
#include "engine/shInput.h"

#include "scene/shScene.h"

#include "shfd/shFd.h"

typedef struct ShEngine {
    ShVkCore        core;
    ShWindow        window;
    ShTime          time;
    ShScene         scenes[1];
    uint32_t        scene_count;
    ShPhysicsHost   physics_host;
}ShEngine;

static void shReloadMaterialInfos(const ShFd mat_info_descriptor, uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos) {
	shMaterialInfosRelease(p_mat_info_count, pp_mat_infos);
	shLoadMaterialInfos(mat_info_descriptor.path, p_mat_info_count, pp_mat_infos);
}

static void shReloadScene(ShEngine* p_engine, const uint32_t scene_idx, const ShFd scene_descriptor, ShMaterialInfo* p_mat_infos) {
	shSceneRelease(p_engine, scene_idx);
	shLoadScene(scene_descriptor.path, p_mat_infos, &p_engine->scenes[scene_idx]);
	shSceneInit(p_engine, scene_idx);
}

static void shReloadPhysicsWorld(const ShFd physics_descriptor, ShScene* p_scene, ShPhysicsHost* p_host) {
	ShPhysicsHostRelease(p_host);
	shLoadPhysicsWorld(physics_descriptor.path, p_host);
}

#endif//SH_ENGINE_H