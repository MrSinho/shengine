/**
 * @file shEngineUI.h
 * 
 * @brief Declares functions related to setting up the graphical user interface (GUI) for the `shengine` module.
 */

#ifndef SH_ENGINE_GUI_H
#define SH_ENGINE_GUI_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include <shgui/shgui.h>



typedef struct ShEngine ShEngine;



/**
 * @brief Sets up the graphical user interface for the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineGuiSetup(
    ShEngine* p_engine
);

/**
 * @brief Releases the graphical user interface and frees gui-related memory.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * @return Integer status code indicating the success or failure of the operation.
 */
extern uint8_t shEngineGuiRelease(
    ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_GUI_H
