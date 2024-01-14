/**
 * @file shEngine.h
 * @brief Declares the @ref ShEngine structure and associated functions for managing the engine.
 *
 * It contains and manages the main information about the engine state,
 * window properties, timing, profiling, and various other configurations.
 */

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
#include "shengine/shEngineUI.h"
#include "shapplication-host/shApplicationHost.h"
#include "shenvironment/shEnvironment.h"
#include "shenvironment/shFileUtilities.h"

//#include <shthreads/shthreads.h>
#include <smd/smd.h>



/**
 * @brief Number of swapchain images in the engine.
 */
#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT          2

/**
 * @brief Maximum number of swapchain images in the engine.
 */
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT      6

/**
 * @brief Number of renderpass attachments in the engine.
 */
#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT    3

/**
 * @brief Number of color attachments in a subpass in the engine.
 */
#define SH_ENGINE_SUBPASS_COLOR_ATTACHMENT_COUNT 1

/**
 * @brief Boolean value representing true.
 */
#ifndef SH_TRUE
#ifndef __cplusplus
#define SH_TRUE 1
#else
#define SH_TRUE true
#endif//__cplusplus
#endif//SH_TRUE

/**
 * @brief Boolean value representing false.
 */
#ifndef SH_FALSE
#ifndef __cplusplus
#define SH_FALSE 0
#else
#define SH_FALSE false
#endif//__cplusplus
#endif//SH_FALSE

/**
 * @struct ShEngine
 * @brief Represents the ShEngine structure, which is the main instance of the engine.
 */
typedef struct ShEngine {
    ShEngineVkCore           core;                     /**< Vulkan core instance. */
    ShWindow                 window;                   /**< Window properties. */
    ShTime                   time;                     /**< Timing information. */
    ShProfilingTimer         profiling_timer;          /**< Profiling timer. */
    SmdFileHandle            ini_smd;                  /**< SMD file handle for initialization. */
    SmdFileHandle            application_smd;          /**< SMD file handle for application-specific data. */
    SmdFileHandle            host_memory_smd;          /**< SMD file handle for host memory. */
    SmdFileHandle            vulkan_memory_smd;        /**< SMD file handle for Vulkan memory. */
    SmdFileHandle            serial_smd;               /**< SMD file handle for serialization. */
    SmdFileHandle            scene_smd;                /**< SMD file handle for scene-related data. */
    ShIniProperties          ini_properties;           /**< Initialization properties. */
    ShApplicationProperties  application_properties;   /**< Application properties. */
    ShHostMemoryProperties   host_memory_properties;   /**< Host memory properties. */
    ShVulkanMemoryProperties vulkan_memory_properties; /**< Vulkan memory properties. */
    ShSerialProperties       serial_properties;        /**< Serialization properties. */
    ShSceneProperties        scene_properties;         /**< Scene properties. */
    uint8_t                  load_shared;              /**< Flag indicating whether to load shared resources. */
    ShApplicationHost        application_host;         /**< Application host information. */
    //ShThreadPool             thread_pool;              /**< Thread pool for parallel processing. */
    ShVkPipelinePool         pipeline_pool;            /**< Vulkan pipeline pool. */
    ShGui                    gui;                      /**< Main gui structure. */
    void*                    p_ext;                    /**< Additional extension pointer. */
} ShEngine;

/**
 * @brief Allocates memory for a new ShEngine instance.
 *
 * @return A pointer to the allocated ShEngine structure.
 */
#define shAllocateEngine() (ShEngine*)calloc(1, sizeof(ShEngine));

/**
 * @brief Frees the memory of a ShEngine instance.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 */
#define shFreeEngine(p_engine) free(p_engine); (p_engine) = NULL

/**
 * @brief Generates an error message for the `shengine` module.
 *
 * @param condition The condition to check.
 * @param msg The error message.
 * @param failure_expression The expression to execute in case of failure.
 */
#define shEngineError(condition, msg, failure_expression) \
    if ((int)(condition)) { printf("shengine error: %s.\n", msg); failure_expression; }

/**
 * @brief Generates an error message for the ShApplication.
 *
 * @param condition The condition to check.
 * @param msg The error message.
 * @param failure_expression The expression to execute in case of failure.
 */
#define shApplicationError(condition, msg, failure_expression) \
    if ((int)(condition)) { printf("shapplication error: %s. \n", msg); failure_expression; }

/**
 * @brief Represents the engine being ready.
 */
#define SH_ENGINE_READY     1

/**
 * @brief Represents the engine not being ready.
 */
#define SH_ENGINE_NOT_READY 0



/**
 * @brief Sets up Vulkan for the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineSetupVulkan(
    ShEngine* p_engine 
);

/**
 * @brief Allocates a profiling handle for the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shAllocateProfilingHandle(
    ShEngine* p_engine 
);

/**
 * @brief Sets the state of the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shSetEngineState(
    ShEngine* p_engine 
);

/**
 * @brief Resets the state of the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shResetEngineState(
    ShEngine* p_engine 
);

/**
 * @brief Safely sets the state of the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineSafeState(
    ShEngine* p_engine 
);

/**
 * @brief Resizes the frame in the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineFrameResize(
    ShEngine* p_engine 
);

/**
 * @brief Updates the profiling in the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineProfilingUpdate(
    ShEngine* p_engine 
);

/**
 * @brief Updates Vulkan in the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineVulkanUpdate(
    ShEngine* p_engine 
);

/**
 * @brief Updates the state of the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineUpdateState(
    ShEngine* p_engine 
);

/**
 * @brief Manages the state of the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * @param ready State of the engine.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineManageState(
    ShEngine* p_engine, 
    uint8_t   ready   
);

/**
 * @brief Releases Vulkan resources in the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * 
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineVulkanRelease(
    ShEngine* p_engine 
);

/**
 * @brief Releases resources in the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 */
extern uint8_t shEngineRelease(
    ShEngine* p_engine 
);

/**
 * @brief Shuts down the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 */
extern void shEngineShutdown(
    ShEngine* p_engine 
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_H