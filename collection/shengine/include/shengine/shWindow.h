/**
 * @file shWindow.h
 * @brief Handling window-related functionalities in the `shengine` framework.
 *
 * The shWindow.h file contains structures and functions related to handling windows in the `shengine` framework.
 */

#ifndef SH_WINDOW_H
#define SH_WINDOW_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#define GLFW_INCLUDE_NONE
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdint.h>



#include "shengine/shInput.h"



/**
 * @struct ShInput
 * @brief Structure representing input in the `shengine` framework.
 *
 * The ShInput structure represents input in the `shengine` framework, including properties such as cursor position,
 * cursor position changes, mouse events, and key events.
 */
typedef struct ShInput {
    float           cursor_pos_x;   /**< X-coordinate of the cursor position. */
    float           cursor_pos_y;   /**< Y-coordinate of the cursor position. */
    float           d_cursor_pos_x; /**< Change in the X-coordinate of the cursor position. */
    float           d_cursor_pos_y; /**< Change in the Y-coordinate of the cursor position. */
    shMouseEvents   mouse_events;   /**< Mouse events. */
    shMouseEvents   d_mouse_events; /**< Change in mouse events. */
    shKeyEvents     key_events;     /**< Key events. */
    shKeyEvents     d_key_events;   /**< Change in key events. */
} ShInput;



/**
 * @struct ShWindow
 * @brief Structure representing a window in the `shengine` framework.
 *
 * The ShWindow structure represents a window in the `shengine` framework, including properties such as the GLFW window,
 * default cursors, cursor icons, width, height, title, instance extensions, instance extension count, input, and surface
 * resize status.
 */
typedef struct ShWindow {
    GLFWwindow*     window;                          /**< GLFW window. */
    GLFWcursor*     default_cursors[4];              /**< Default cursors. */
    int32_t         default_cursor_icons[4];         /**< Default cursor icons. */
    uint32_t        width;                           /**< Width of the window. */
    uint32_t        height;                          /**< Height of the window. */
    const char*     title;                           /**< Title of the window. */
    const char**    pp_instance_extensions;          /**< Instance extensions. */
    uint32_t        instance_extension_count;        /**< Instance extension count. */
    ShInput         input;                           /**< Input structure. */
    uint8_t         surface_resize_pending;          /**< Surface resize status. */
} ShWindow;



typedef struct ShEngine ShEngine;



/**
 * @brief Sets up a window in the `shengine` framework.
 *
 * @param title     Title of the window.
 * @param p_window  Valid pointer to a @ref ShWindow structure.
 * @return          1 on success, 0 on failure.
 */
extern uint8_t shWindowSetup(
    const char* title,
    ShWindow*   p_window
);

/**
 * @def shSetCursor
 * @brief Macro to set the cursor of a GLFW window.
 *
 * The shSetCursor macro sets the cursor of a GLFW window to a specified cursor.
 *
 * Example:
 * @code
 * shSetCursor(glfw_window, glfw_cursor)
 * @endcode
 *
 * @param glfw_window GLFW window.
 * @param glfw_cursor GLFW cursor.
 */
#define shSetCursor(glfw_window, glfw_cursor)\
    glfwSetCursor(glfw_window, glfw_cursor)

/**
 * @def shPollEvents
 * @brief Macro to poll events in the GLFW window.
 *
 * The shPollEvents macro polls events in the GLFW window.
 */
#define shPollEvents glfwPollEvents

/**
 * @def shWaitEvents
 * @brief Macro to wait for events in the GLFW window.
 *
 * The shWaitEvents macro waits for events in the GLFW window.
 */
#define shWaitEvents glfwWaitEvents

/**
 * @brief Clears the contents of a window in the `shengine` framework.
 *
 * @param p_window Valid pointer to a @ref ShWindow structure.
 * @return         1 on success, 0 on failure.
 */
extern uint8_t shClearWindow(
    ShWindow* p_window
);

/**
 * @def shIsWindowActive
 * @brief Macro to check if a window is active.
 *
 * The shIsWindowActive macro checks if a GLFW window is active.
 *
 * @param _window ShWindow structure to check for activity.
 * @return        True if the window is active, false otherwise.
 */
#define shIsWindowActive(_window)\
    (!glfwWindowShouldClose((_window).window))

/**
 * @brief Creates a surface for the `shengine` module using the window.
 *
 * The `shWindowCreateSurface` function creates a surface for the `shengine` module using the window.
 *
 * @param p_engine Pointer to a valid @ref ShEngine structure.
 * @return An integer indicating the success (1) or failure (0) of creating the window surface.
 */
extern uint8_t shWindowCreateSurface(
    ShEngine* p_engine
);

/**
 * @brief Updates input for the specified window.
 *
 * The `shUpdateInput` function updates input for the specified ShWindow.
 *
 * @param p_window Pointer to the ShWindow structure to update input.
 * @return An integer indicating the success (1) or failure (0) of updating input.
 */
extern uint8_t shUpdateInput(
    ShWindow* p_window
);

/**
 * @brief Retrieves the size of the specified window.
 *
 * The `shGetWindowSize` function retrieves the size of the specified ShWindow.
 *
 * @param p_window Pointer to the ShWindow structure to get the size.
 * @return An integer indicating the success (1) or failure (0) of retrieving the window size.
 */
extern uint8_t shGetWindowSize(
    ShWindow* p_window
);

/**
 * @brief Updates the specified window within the `shengine` module.
 *
 * The `shUpdateWindow` function updates the specified ShWindow within `shengine` module.
 *
 * @param p_engine Pointer to a valid @ref ShEngine structure.
 * @return An integer indicating the success (1) or failure (0) of updating the window.
 */
extern uint8_t shUpdateWindow(
    ShEngine* p_engine
);



/**
 * @brief Checks if a key is pressed in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param key    Key code to check.
 * @return       True if the key is pressed, false otherwise.
 */
static uint8_t shIsKeyPressed(const ShWindow window, const uint32_t key) {
	return window.input.d_key_events[key] == GLFW_PRESS;
}

/**
 * @brief Checks if a key is down in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param key    Key code to check.
 * @return       True if the key is down, false otherwise.
 */
static uint8_t shIsKeyDown(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_PRESS;
}

/**
 * @brief Checks if a key is released in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param key    Key code to check.
 * @return       True if the key is released, false otherwise.
 */
static uint8_t shIsKeyReleased(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_RELEASE;
}

/**
 * @brief Checks if a key is repeated in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param key    Key code to check.
 * @return       True if the key is repeated, false otherwise.
 */
static uint8_t shIsKeyRepeated(const ShWindow window, const uint32_t key) {
	return window.input.key_events[key] == GLFW_REPEAT;
}

/**
 * @brief Checks if a mouse button is pressed in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param button Mouse button code to check.
 * @return       True if the mouse button is pressed, false otherwise.
 */
static uint8_t shIsMouseButtonPressed(const ShWindow window, const uint32_t button) {
	return window.input.d_mouse_events[button] == GLFW_PRESS;
}

/**
 * @brief Checks if a mouse button is down in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param button Mouse button code to check.
 * @return       True if the mouse button is down, false otherwise.
 */
static uint8_t shIsMouseButtonDown(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_PRESS;
}

/**
 * @brief Checks if a mouse button is released in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param button Mouse button code to check.
 * @return       True if the mouse button is released, false otherwise.
 */
static uint8_t shIsMouseButtonReleased(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_RELEASE;
}

/**
 * @brief Checks if a mouse button is repeated in a window.
 *
 * @param window Valid pointer to a @ref ShWindow structure.
 * @param button Mouse button code to check.
 * @return       True if the mouse button is repeated, false otherwise.
 */
static uint8_t shIsMouseButtonRepeated(const ShWindow window, const uint32_t button) {
	return window.input.mouse_events[button] == GLFW_REPEAT;
}



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_WINDOW_H
