/**
 * @file
 * @brief Declares the shEditorMain function for interacting with the engine from the editor.
 *
 * The shEditorMain function is responsible for the main functionality of the editor, interacting with the @ref ShEngine structure.
 */

#ifndef SH_EDITOR_H
#define SH_EDITOR_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shEngine.h"



/**
 * @brief Main function for the editor to interact with the `shengine` module.
 *
 * @param p_engine Valid pointer to a @ref ShEngine structure.
 * @return Integer status code indicating the success or failure of the operation.
 */
extern int shEditorMain(
    ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_EDITOR_H
