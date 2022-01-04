#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#include <shVkCore.h>

#include "ecs/shEcsImplementation.h"
#include "ecs/shPhysicsInfo.h"

#include "engine/shWindow.h"
#include "engine/shTime.h"

#include <stdint.h>

typedef struct ShEngine {
    ShVkCore        core;
    ShWindow        window;
    ShTime          time;
    ShScene         scenes[1];
    uint32_t        scene_count;
    ShPhysicsHost   physics_host;
}ShEngine;

#endif//SH_ENGINE_H