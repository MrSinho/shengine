#ifndef SH_ENGINE_H
#define SH_ENGINE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "shengine/shEngineVkCore.h"

#include "shengine/shWindow.h"
#include "shengine/shTime.h"
#include "shengine/shProfiling.h"
#include "shengine/shInput.h"
#include "shengine/shScene.h"

#include "shapplicationhost/shApplicationHost.h"

#include "shenvironment/shEnvironment.h"
#include "shenvironment/shFileUtilities.h"

#include <shthreads/shthreads.h>
#include <smd/smd.h>



#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT         2
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT     6
#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT   3
#define SH_ENGINE_SUBASS_COLOR_ATTACHMENT_COUNT 1


#ifndef SH_TRUE
#ifndef __cplusplus
#define SH_TRUE 1
#else
#define SH_TRUE true
#endif//__cplusplus
#endif//SH_TRUE

#ifndef SH_FALSE
#ifndef __cplusplus
#define SH_FALSE 0
#else
#define SH_FALSE false
#endif//__cplusplus
#endif//SH_FALSE



typedef struct ShEngine {
    ShEngineVkCore           core;
						     
    ShWindow                 window;
    ShTime                   time;
	
	ShProfilingTimer         profiling_timer;

    SmdFileHandle*           p_ini_smd;
	SmdFileHandle*           p_application_smd;
	SmdFileHandle*           p_host_memory_smd;
	SmdFileHandle*           p_vulkan_memory_smd;
	SmdFileHandle*           p_serial_smd;
	SmdFileHandle*           p_scene_smd;
	
	ShIniProperties          ini_properties;
	ShApplicationProperties  application_properties;
	ShHostMemoryProperties   host_memory_properties;
	ShVulkanMemoryProperties vulkan_memory_properties;
	ShSerialProperties       serial_properties;
	ShSceneProperties        scene_properties;

	uint8_t                  load_shared;
    ShApplicationHost        application_host;
	
	ShThreadPool             thread_pool;

	ShVkPipelinePool*        p_pipeline_pool;
	uint32_t                 pipeline_count;
							 	 
    void*                    p_ext;
} ShEngine;

#define shAllocateEngine() (ShEngine*)calloc(1, sizeof(ShEngine));
#define shFreeEngine(p_engine) free(p_engine); (p_engine) = NULL



static uint8_t shEngineWarning(int condition, const char* msg) {
    if ((int)(condition)) { printf("shengine warning: %s.\n", msg); return 1; }
    return 0;
}

#define shEngineError(condition, msg, failure_expression)\
	if ((int)(condition)) { printf("shengine error: %s.\n", msg); failure_expression; }

#define shApplicationError(condition, msg, failure_expression)\
    if ((int)(condition)) { printf("shapplication error: %s. \n", msg); failure_expression; }



#define SH_ENGINE_READY     1
#define SH_ENGINE_NOT_READY 0


extern uint8_t shEngineSetupVulkan(
	ShEngine* p_engine
);

extern uint8_t shAllocateProfilingHandle(
	ShEngine* p_engine
);

extern uint8_t shSetEngineState(
	ShEngine* p_engine
);

extern uint8_t shResetEngineState(
	ShEngine* p_engine
);

extern uint8_t shEngineSafeState(
	ShEngine* p_engine
);

extern uint8_t shEngineFrameResize(
	ShEngine* p_engine
);

extern uint8_t shEngineProfilingUpdate(
	ShEngine* p_engine
);

extern uint8_t shEngineVulkanUpdate(
	ShEngine* p_engine
);

extern uint8_t shEngineUpdateState(
	ShEngine* p_engine
);

extern uint8_t shEngineManageState(
	ShEngine* p_engine,
	uint8_t   ready
);

extern uint8_t shEngineVulkanRelease(
	ShEngine* p_engine
);

extern uint8_t shEngineRelease(
	ShEngine* p_engine
);

extern void shEngineShutdown(
	ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H