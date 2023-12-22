/**
 * @file shProfiling.h
 * @brief Profiling functionality for the `shengine` module.
 *
 * The shProfiling.h file contains structures and functions related to profiling and timing
 * within the `shengine` framework.
 */

#ifndef SH_PROFILING_H
#define SH_PROFILING_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shEngineVkCore.h"
#include <smd/smd.h>



/**
 * @brief Maximum number of extension timers supported.
 */
#define SH_PROFILING_TIMER_MAX_EXT_COUNT 1024



/**
 * @enum ShProfilingTimerType
 * @brief Types of profiling timers used by the `shengine` library.
 *
 * The `ShProfilingTimerType` enumeration defines various types of profiling timers used by the `shengine` library.
 * Each type corresponds to a specific aspect of the engine's execution.
 */
typedef enum ShProfilingTimerType {
    SH_PROFILING_TIMER_MAIN_THREAD                 = 0, /**< Main thread timer. */
    SH_PROFILING_TIMER_APPLICATION_UPDATE          = 1, /**< Application update timer. */
    SH_PROFILING_TIMER_APPLICATION_MAIN_CMD_BUFFER = 2, /**< Application main command buffer timer. */
    SH_PROFILING_TIMER_APPLICATION_MAIN_RENDERPASS    , /**< Application main render pass timer. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_0         , /**< Main command buffer wait timer 0. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_1         , /**< Main command buffer wait timer 1. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_2         , /**< Main command buffer wait timer 2. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_3         , /**< Main command buffer wait timer 3. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_4         , /**< Main command buffer wait timer 4. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_5         , /**< Main command buffer wait timer 5. */
    SH_PROFILING_TIMER_MAIN_CMD_BUFFER_WAIT_6         , /**< Main command buffer wait timer 6. */
    SH_PROFILING_TIMER_EXT                            , /**< extension timer type. */
    SH_PROFILING_TIMER_MAX_ENUM                       , /**< Maximum value for enumeration. */
} ShProfilingTimerType;



/**
 * @struct ShProfilingTimer
 * @brief Structure to hold profiling timer information in ShEngine.
 *
 * The ShProfilingTimer structure is used to store timing information for different aspects of ShEngine execution.
 * It includes timing data for the main thread, application update, command buffer execution, rendering pass,
 * and extension timers, among other things.
 */
typedef struct ShProfilingTimer {
    double          main_profiling_tool_last_time; /**< Last time profiling tool was queried. */

    double          main_thread_start_s;           /**< Start time of the main thread. */
    double          main_thread_end_s;             /**< End time of the main thread. */
    double          main_thread_dtime_ms;          /**< Duration of the main thread in milliseconds. */

    double          main_graphics_queue_start_s;   /**< Start time of the main graphics queue. */
    double          main_graphics_queue_end_s;     /**< End time of the main graphics queue. */
    double          main_graphics_queue_dtime_ms;  /**< Duration of the main graphics queue in milliseconds. */

    double          application_update_start_s;    /**< Start time of the application update. */
    double          application_update_end_s;      /**< End time of the application update. */
    double          application_update_dtime_ms;   /**< Duration of the application update in milliseconds. */

    double          application_main_cmd_buffer_start_s;  /**< Start time of the application main command buffer. */
    double          application_main_cmd_buffer_end_s;    /**< End time of the application main command buffer. */
    double          application_main_cmd_buffer_dtime_ms; /**< Duration of the application main command buffer in milliseconds. */

    double          application_main_renderpass_start_s;  /**< Start time of the application main render pass. */
    double          application_main_renderpass_end_s;    /**< End time of the application main render pass. */
    double          application_main_renderpass_dtime_ms; /**< Duration of the application main render pass in milliseconds. */

    double          main_cmd_buffer_wait_start_s[SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT]; /**< Start times of main command buffer waits. */
    double          main_cmd_buffer_wait_end_s  [SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT]; /**< End times of main command buffer waits. */
    double          main_cmd_buffer_wait_dtime_s[SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT]; /**< Durations of main command buffer waits in seconds. */

    uint32_t        ext_count;                                      /**< Count of extension timers. */
    double          ext_start_s [SH_PROFILING_TIMER_MAX_EXT_COUNT]; /**< Start times of extension timers. */
    double          ext_end_s   [SH_PROFILING_TIMER_MAX_EXT_COUNT]; /**< End times of extension timers. */
    double          ext_dtime_ms[SH_PROFILING_TIMER_MAX_EXT_COUNT]; /**< Durations of extension timers in milliseconds. */
    SmdVarName      ext_names   [SH_PROFILING_TIMER_MAX_EXT_COUNT]; /**< Names of extension timers. */

    SmdExportHandle export; /**< Export handle for serialization. */
} ShProfilingTimer;



/**
 * @brief Starts the specified profiling timer.
 *
 * @param p_timer Pointer to a valid @ref ShProfilingTimer structure.
 * @param type    Type of the profiling timer to start.
 * 
 * @return        1 on success, 0 on failure.
 */
extern uint8_t shProfilingTimerStart(
    ShProfilingTimer*    p_timer,
    ShProfilingTimerType type
);

/**
 * @brief Ends the specified profiling timer.
 *
 * @param p_timer Pointer to a valid @ref ShProfilingTimer structure.
 * @param type    Type of the profiling timer to end.
 * 
 * @return        1 on success, 0 on failure.
 */
extern uint8_t shProfilingTimerEnd(
    ShProfilingTimer*    p_timer,
    ShProfilingTimerType type
);

/**
 * @brief Sets the count of extension timers in the ShProfilingTimer structure.
 *
 * @param p_timer   Pointer to a valid @ref ShProfilingTimer structure.
 * @param ext_count Count of extension timers.
 * 
 * @return          1 on success, 0 on failure.
 */
extern uint8_t shProfilingTimerSetExtCount(
    ShProfilingTimer* p_timer,
    uint32_t          ext_count
);

/**
 * @brief Starts an extension profiling timer with the specified name and index.
 *
 * @param p_timer   Pointer to a valid @ref ShProfilingTimer structure.
 * @param name      Name of the extension timer.
 * @param timer_idx Index of the extension timer.
 * 
 * @return          1 on success, 0 on failure.
 */
extern uint8_t shProfilingTimerStartExt(
    ShProfilingTimer* p_timer,
    SmdVarName        name,
    uint32_t          timer_idx
);

/**
 * @brief Ends an extension profiling timer with the specified index.
 *
 * @param p_timer   Pointer to a valid @ref ShProfilingTimer structure.
 * @param timer_idx Index of the extension timer.
 * 
 * @return          1 on success, 0 on failure.
 */
extern uint8_t shProfilingTimerEndExt(
    ShProfilingTimer*  p_timer,
    uint32_t          timer_idx
);



#ifdef __cplusplus
}
#endif//__cplusplus
#endif//SH_PROFILING_H
