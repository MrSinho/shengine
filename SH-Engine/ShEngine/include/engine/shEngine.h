#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#include <shvulkan/shVkCore.h>

#include "ecs/shEcsImplementation.h"
#include "ecs/shPhysics.h"
#include "ecs/shMaterial.h"

#include "engine/shWindow.h"
#include "engine/shTime.h"
#include "engine/shInput.h"

#include "scene/shScene.h"

#include "shfd/shFd.h"

#include <stdint.h>

typedef struct ShEngine {
    ShVkCore        core;
    ShWindow        window;
    ShTime          time;
    ShScene         scenes[1];
    uint32_t        scene_count;
    ShPhysicsHost   physics_host;
}ShEngine;

static void shReloadMaterialInfos(const ShFd mat_info_descriptor, uint32_t* p_mat_info_count, ShMaterial** pp_materials) {
	shMaterialsRelease(p_mat_info_count, pp_materials);
	shLoadMaterials(mat_info_descriptor.path, p_mat_info_count, pp_materials);
}

static void shReloadScene(ShEngine* p_engine, const uint32_t scene_idx, const ShFd scene_descriptor, ShMaterial* p_materials) {
	shSceneRelease(p_engine, scene_idx);
	shLoadScene(scene_descriptor.path, p_materials, &p_engine->scenes[scene_idx]);
	shSceneInit(p_engine, scene_idx);
}

static void shReloadPhysicsWorld(const ShFd physics_descriptor, ShScene* p_scene, ShPhysicsHost* p_host) {
	ShPhysicsHostRelease(p_host);
	shLoadPhysicsWorld(physics_descriptor.path, p_host);
}

#endif//SH_ENGINE_H