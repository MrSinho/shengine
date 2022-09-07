#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shvulkan/shVkCore.h>
#include <shthreads/shthreads.h>
#include <shgui/shgui.h>

#include "shecs/shEcsImplementation.h"
#include "shecs/shMaterial.h"

#include "shscene/shScene.h"

#include "shengine/shWindow.h"
#include "shengine/shTime.h"
#include "shengine/shInput.h"

#include "shsharedhost/shSharedHost.h"
#include "shfd/shFd.h"

typedef enum ShEngineExtensionSignal {
    SH_ENGINE_SHUTTING_DOWN = 0,
    SH_ENGINE_RUNNING       = 1,
} ShEngineExtensionSignal;

typedef struct ShEngine {
    ShVkCore              core;
    ShWindow              window;
    ShTime                time;
    ShFd                  materials_descriptor;
    ShFd                  scene_descriptor;
    ShFd                  simulation_descriptor;
    ShScene               scene;
    uint32_t              scene_count;
	ShMaterialHost*       p_materials;
    ShSimulationHandle    simulation_host;
    ShThreadsHandle       threads_handle;
    uint32_t              material_count;
    ShGui*                p_gui;
    
    void*                 p_ext;

} ShEngine;

#define SH_SIMULATION_THREAD_IDX 0



static uint8_t shEngineWarning(int condition, const char* msg) {
    if ((int)(condition)) { printf("shengine warning: %s.\n", msg); return 1; }
    return 0;
}

#define shEngineError(condition, msg)\
	if ((int)(condition)) { printf("shengine error: %s.\n", msg); exit(-1); }



extern void shEngineSafeState(ShEngine* p_engine);

extern uint8_t shSetEngineState(ShEngine* p_engine);

extern uint8_t shResetEngineState(ShEngine* p_engine, const uint8_t release_shared);

extern void shEngineUpdateState(ShEngine* p_engine);

extern void shEngineManageState(ShEngine* p_engine, const uint8_t ready, const uint8_t release_on_failure);

extern void shEngineRelease(ShEngine* p_engine, const uint8_t release_shared);

extern void shEngineShutdown(ShEngine* p_engine);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H