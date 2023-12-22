/**
 * @file
 * @brief Platform-specific shared library loading functions.
 *
 * This file provides platform-specific macros and functions for loading shared libraries dynamically.
 */

#ifndef SH_SHARED_HOST_H
#define SH_SHARED_HOST_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#ifdef _MSC_VER
#pragma warning (disable: 4005 4996 4113)
#endif//_MSC_VER


#ifdef _WIN32
#include <windows.h>

/**
 * @brief Macro to load a shared library on Windows.
 *
 * This macro uses the Windows API function `LoadLibrary` to load a shared library.
 *
 * @param path Path to the shared library.
 * @return A handle to the loaded shared library.
 */
#define shLoadShared(path) LoadLibrary(path)

/**
 * @brief Macro to load a symbol from a shared library on Windows.
 *
 * This macro uses the Windows API function `GetProcAddress` to load a symbol from a shared library.
 *
 * @param handle Handle to the loaded shared library.
 * @param symbol Symbol name to load.
 * @return A pointer to the loaded symbol.
 */
#define shSharedLoadSymbol(handle, symbol) GetProcAddress(handle, symbol)

/**
 * @brief Macro to free a loaded shared library on Windows.
 *
 * This macro uses the Windows API function `FreeLibrary` to free a loaded shared library.
 *
 * @param handle Handle to the loaded shared library.
 */
#define shSharedFree(handle) FreeLibrary(handle)

#else
#include <dlfcn.h>

/**
 * @brief Macro to load a shared library on non-Windows platforms.
 *
 * This macro uses the POSIX function `dlopen` to load a shared library.
 *
 * @param path Path to the shared library.
 * @return A handle to the loaded shared library.
 */
#define shLoadShared(path) dlopen(path, RTLD_LAZY)

/**
 * @brief Macro to load a symbol from a shared library on non-Windows platforms.
 *
 * This macro uses the POSIX function `dlsym` to load a symbol from a shared library.
 *
 * @param handle Handle to the loaded shared library.
 * @param symbol Symbol name to load.
 * @return A pointer to the loaded symbol.
 */
#define shSharedLoadSymbol(handle, symbol) dlsym(handle, symbol)

/**
 * @brief Macro to free a loaded shared library on non-Windows platforms.
 *
 * This macro uses the POSIX function `dlclose` to free a loaded shared library.
 *
 * @param handle Handle to the loaded shared library.
 */
#define shSharedFree(handle) dlclose(handle)

#endif // _WIN32

#include <stdint.h>
#include <stddef.h>

/**
 * @typedef ShSharedHandle
 * @brief Represents a handle to a loaded shared library.
 */
typedef void* ShSharedHandle;

/**
 * @typedef ShApplicationFunc
 * @brief Represents the function signature for the application's main function. 
 * 
 * It takes A pointer as argument to manage some data that the application may use.
 * @return An 8-bit unsigned integer representing the success or failure of the application.
 */
typedef uint8_t(ShApplicationFunc) (void*);

/**
 * @typedef ShApplicationThreadFunc
 * @brief Represents the function signature for a thread within the application.
 *
 * It takes A pointer as argument to manage some data that the application may use.
 * @return A 64-bit unsigned integer representing the success or failure of the thread.
 */
typedef uint64_t (ShApplicationThreadFunc) (void*);

/**
 * @struct ShApplicationHost
 * @brief Represents the host for a shared application.
 */
typedef struct ShApplicationHost {
    ShSharedHandle           shared;              /**< Handle to the loaded shared library. */
    ShApplicationFunc*       p_start;             /**< Pointer to the application's start function. */
    ShApplicationFunc*       p_update;            /**< Pointer to the application's update function. */
    ShApplicationFunc*       p_main_cmd_buffer;   /**< Pointer to the application's main command buffer function. */
    ShApplicationFunc*       p_main_renderpass;   /**< Pointer to the application's main render pass function. */
    ShApplicationFunc*       p_frame_resize;      /**< Pointer to the application's frame resize function. */
    ShApplicationFunc*       p_close;             /**< Pointer to the application's close function. */
} ShApplicationHost;

/**
 * @brief Macro to handle errors in the shared host.
 *
 * This macro prints an error message and executes a failure expression if a condition is true.
 *
 * @param condition The condition to check.
 * @param msg The error message.
 * @param failure_expression The expression to execute in case of failure.
 */
#define shSharedHostError(condition, msg, failure_expression)\
    if ((int)(condition)) { printf("shsharedhost error: %s\n", msg); failure_expression; }

/**
 * @brief Runs a shared application.
 *
 * This function runs the shared application by executing the provided application function.
 *
 * @param p_engine A pointer to the engine data.
 * @param p_func The application function to run.
 * @return A 64-bit unsigned integer representing the success or failure of the application.
 */
extern uint64_t shApplicationRun(
    void*              p_engine,
    ShApplicationFunc* p_func
);

/**
 * @brief Retrieves the shared application information.
 *
 * This function retrieves information about the shared application from the shared library.
 *
 * @param shared_name The name of the shared library.
 * @param s_start The symbol name for the start function.
 * @param s_update The symbol name for the update function.
 * @param s_main_cmd_buffer The symbol name for the main command buffer function.
 * @param s_main_renderpass The symbol name for the main render pass function.
 * @param s_frame_resize The symbol name for the frame resize function.
 * @param s_close The symbol name for the close function.
 * @param p_application A pointer to the application host structure to fill.
 * @return An 8-bit unsigned integer representing the success or failure of retrieving the shared application.
 */
extern uint8_t shGetSharedApplication(
    const char*          shared_name,
    const char*          s_start,
    const char*          s_update,
    const char*          s_main_cmd_buffer,
    const char*          s_main_renderpass,
    const char*          s_frame_resize,
    const char*          s_close,
    ShApplicationHost*   p_application
);

/**
 * @brief Releases the shared library.
 *
 * This macro frees the loaded shared library if it is not NULL.
 *
 * @param p_shared A pointer to the shared library handle.
 */
#define shSharedRelease(p_shared)\
    if ((p_shared) != NULL) { if (*(p_shared) != NULL) { shSharedFree(*(p_shared)); *(p_shared) = NULL; } }



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SHARED_HOST_H
