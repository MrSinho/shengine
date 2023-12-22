/**
 * @file shScene.h
 * @brief Handling scenes in the `shengine` framework.
 *
 * The shScene.h file contains structures and functions related to handling scenes in the `shengine` framework.
 */

#ifndef SH_SCENE_HANDLE_H
#define SH_SCENE_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



typedef struct ShEngine ShEngine;
typedef struct ShIdentity ShIdentity;
typedef struct ShCamera ShCamera;
typedef struct ShTransform ShTransform;
typedef struct ShHostMemoryLinker ShHostMemoryLinker;
typedef struct ShSceneProperties ShSceneProperties;



/**
 * @brief Initializes the scene in the `shengine` framework.
 *
 * @param p_engine Pointer to a valid @ref ShEngine structure.
 * @return         1 on success, 0 on failure.
 */
extern uint8_t shSceneInit(
    ShEngine* p_engine
);

/**
 * @brief Updates the transform in the `shengine` framework.
 *
 * @param p_transform Pointer to a valid @ref ShTransform structure.
 * @return            1 on success, 0 on failure.
 */
extern uint8_t shUpdateTransform(
    ShTransform* p_transform
);

/**
 * @brief Updates the camera in the `shengine` framework.
 *
 * @param p_engine    Pointer to a valid @ref ShEngine structure.
 * @param p_transform Pointer to a valid @ref ShTransform structure.
 * @param p_camera    Pointer to a valid @ref ShCamera structure.
 * @return            1 on success, 0 on failure.
 */
extern uint8_t shUpdateCamera(
    ShEngine*    p_engine,
    ShTransform* p_transform,
    ShCamera*    p_camera
);

/**
 * @brief Updates the scene in the `shengine` framework.
 *
 * @param p_engine Pointer to a valid @ref ShEngine structure.
 * @return         1 on success, 0 on failure.
 */
extern uint8_t shSceneUpdate(
    ShEngine* p_engine
);

/**
 * @brief Ends the scene in the `shengine` framework.
 *
 * @param p_engine Pointer to a valid @ref ShEngine structure.
 * @return         1 on success, 0 on failure.
 */
extern uint8_t shEndScene(
    ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SCENE_HANDLE_H
