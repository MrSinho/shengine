/**
 * @file
 * @brief Defines the ShCamera structure and related constants and functions.
 *
 * The ShCamera structure represents a camera component for an entity, along with its properties and transformation matrices.
 */

#ifndef SH_CAMERA_H
#define SH_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdint.h>



/**
 * @struct ShCamera
 * @brief Represents a camera component in scene.
 */
typedef struct ShCamera {
    float   fov;                  /**< Field of view. */
    float   nc;                   /**< Near clipping plane. */
    float   fc;                   /**< Far clipping plane. */
    float   speed;                /**< Camera movement speed. */
    float   mouse_speed;          /**< Mouse sensitivity for camera rotation. */
    float   projection[4][4];     /**< Projection matrix. */
    float   view[4][4];           /**< View matrix. */
    uint8_t free_flight;          /**< Flag indicating free flight mode. */
} ShCamera;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_CAMERA_H
