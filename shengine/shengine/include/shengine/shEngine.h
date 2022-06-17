#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <shvulkan/shVkCore.h>

#include "shecs/shEcsImplementation.h"
#include "shecs/shMaterial.h"

#include "shengine/shWindow.h"
#include "shengine/shTime.h"
#include "shengine/shInput.h"

#include "shscene/shScene.h"

#include <shfd/shFd.h>

#include "shsharedhost/shSharedHost.h"

#include <stdint.h>



typedef struct ShEngine {
    ShVkCore            core;
    ShWindow            window;
    ShTime              time;
    ShFd                materials_descriptor;
    ShFd                scene_descriptor;
    ShFd                simulation_descriptor;
    ShScene             scene;
    uint32_t            scene_count;
	ShMaterialHost*	    p_materials;
    ShSimulationHandle  simulation_host;
    uint32_t		    material_count;

    void*               p_engine_extension;
}ShEngine;



static uint8_t shEngineWarning(int condition, const char* msg) {
    if (!(int)(condition)) { printf("shengine warning: %s\n", msg); return 0; }
    return 1;
}

#define shEngineError(condition, msg)\
	if (!(int)(condition)) { printf("shengine error: %s\n", msg); perror("aborting"); }



extern void shEngineSafeState(ShEngine* p_engine);

extern uint8_t shSetEngineState(ShEngine* p_engine);

extern uint8_t shResetEngineState(ShEngine* p_engine);

extern void shEngineUpdateState(ShEngine* p_engine);

extern void shEngineManageState(ShEngine* p_engine, const uint8_t ready);

extern void shEngineRelease(ShEngine* p_engine);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H