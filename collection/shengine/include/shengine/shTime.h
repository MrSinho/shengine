/**
 * @file shTime.h
 * @brief Handling time-related functionalities in the `shengine` framework.
 *
 * The shTime.h file contains structures and functions related to handling time in the `shengine` framework.
 */

#ifndef SH_TIME_H
#define SH_TIME_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



/**
 * @def SH_TIME_MAX_TICK_COUNT
 * @brief Maximum number of ticks in the time structure.
 *
 * SH_TIME_MAX_TICK_COUNT defines the maximum number of ticks that can be tracked in the ShTime structure.
 */
#define SH_TIME_MAX_TICK_COUNT       1024



/**
 * @typedef ShTimeFunc
 * @brief Function type for time-related callbacks.
 *
 * The ShTimeFunc typedef defines a function type for time-related callbacks, taking a void pointer as an argument and
 * returning a uint64_t.
 */
typedef uint64_t ShTimeFunc(void*);



/**
 * @enum ShTickStatus
 * @brief Enumerates possible tick statuses.
 *
 * The ShTickStatus enumeration defines possible statuses of a tick in the ShTime structure, including undefined,
 * initialized, waiting, executing, and finished executing.
 */
typedef enum ShTickStatus {
    SH_TICK_UNDEFINED          = 0, /**< Tick status undefined. */
    SH_TICK_INITIALIZED        = 1, /**< Tick status initialized. */
    SH_TICK_WAITING            = 2, /**< Tick status waiting. */
    SH_TICK_EXECUTING          = 3, /**< Tick status executing. */
    SH_TICK_FINISHED_EXECUTING = 4, /**< Tick status finished executing. */
    SH_TICK_STATUS_MAX_ENUM
} ShTickStatus;



/**
 * @struct ShTime
 * @brief Structure representing time in the `shengine` framework.
 *
 * The ShTime structure represents time in the `shengine` framework, including properties such as the current time,
 * delta time, last time, tick count, tick last time, tick status, and tick functions.
 */
typedef struct ShTime {
    double           now;                          /**< Current time. */
    double           delta_time;                   /**< Delta time. */
    double           last_time;                    /**< Last time. */
    
    uint32_t         tick_count;                   /**< Tick count. */
    double           ticks_last_time[SH_TIME_MAX_TICK_COUNT]; /**< Tick last time. */
    ShTickStatus     ticks_status[SH_TIME_MAX_TICK_COUNT];    /**< Tick status. */
    ShTimeFunc*      p_ticks_funcs[SH_TIME_MAX_TICK_COUNT];    /**< Tick functions. */
} ShTime;



/**
 * @brief Gets the current time in the `shengine` framework.
 *
 * @param p_time Pointer to a valid @ref ShTime structure.
 * @return       1 on success, 0 on failure.
 */
extern uint8_t shGetTime(
    ShTime* p_time
);

/**
 * @brief Sets the time in the `shengine` framework.
 *
 * @param now    Current time to set.
 * @param p_time Pointer to a valid @ref ShTime structure.
 * @return       1 on success, 0 on failure.
 */
extern uint8_t shSetTime(
    double  now,
    ShTime* p_time
);

/**
 * @brief Calls a function on a tick in the `shengine` framework.
 *
 * @param p_time         Pointer to a valid @ref ShTime structure.
 * @param seconds        Time interval in seconds for the tick.
 * @param tick_idx       Index of the tick.
 * @param p_func         Pointer to the time function to call.
 * @param p_arg          Pointer to an argument for the time function.
 * @param p_return_value Pointer to store the return value of the time function.
 * @return               1 on success, 0 on failure.
 */
extern uint8_t shCallOnTick(
    ShTime*     p_time,
    double      seconds,
    uint32_t    tick_idx,
    ShTimeFunc* p_func,
    void*       p_arg,
    uint64_t*   p_return_value
);

/**
 * @def shOnTick
 * @brief Macro to execute an expression on a tick in the `shengine` framework.
 *
 * The shOnTick macro executes the specified expression when a tick condition is met in the ShTime structure.
 *
 * Example:
 * @code
 * shOnTick(time, 0.5, 1, printf("Tick 1 executed!"))
 * @endcode
 *
 * @param time         ShTime structure to check for ticks.
 * @param seconds_d    Time interval in seconds for the tick.
 * @param tick_idx     Index of the tick.
 * @param expression   Expression to execute when the tick condition is met.
 */
#define shOnTick(time, seconds_d, tick_idx, expression)\
    if ((time).ticks_status[tick_idx] == SH_TICK_UNDEFINED) {\
        (time).ticks_status[tick_idx] = SH_TICK_INITIALIZED;\
    }\
    if (\
        (\
            ((time).now - (time).ticks_last_time[tick_idx]) >= (double)(seconds_d)\
        ) &&\
        (tick_idx < SH_TIME_MAX_TICK_COUNT) &&\
        (\
            (time).ticks_status[tick_idx] != SH_TICK_UNDEFINED &&\
            (time).ticks_status[tick_idx] != SH_TICK_EXECUTING\
        )\
    ) {\
        (time).ticks_status[tick_idx] = SH_TICK_EXECUTING;\
        expression;\
        (time).ticks_last_time[tick_idx] = (time).now;\
        (time).ticks_status[tick_idx] = SH_TICK_FINISHED_EXECUTING;\
    }\
    else {\
        (time).ticks_status[tick_idx] = SH_TICK_WAITING;\
    }

/**
 * @brief Sleeps for a specified number of milliseconds.
 *
 * @param ms Number of milliseconds to sleep.
 */
extern void shSleep(
    uint32_t ms
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TIME_H
