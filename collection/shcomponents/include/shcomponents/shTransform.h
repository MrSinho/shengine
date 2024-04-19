/**
 * @file
 * @brief Defines the ShTransform structure for representing transformations in the engine.
 *
 * The ShTransform structure represents a transformation in the engine, including the model matrix, position, rotation, scale, and related vectors.
 */

#ifndef SH_TRANSFORM_H
#define SH_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



typedef float  shvec3[3];
typedef float  shvec4[4];
typedef shvec4 shmat4[4];



/**
 * @struct ShTransform
 * @brief Represents a transformation in the engine.
 */
typedef struct ShTransform {
    shmat4 model;     /**< Model matrix. */
    shvec4 position;  /**< Position vector. */
    shvec4 rotation;  /**< Rotation vector. */
    shvec4 euler;     /**< Euler vector. */
    shvec4 scale;     /**< Scale vector. */
    shvec4 front;     /**< Front vector. */
    shvec4 left;      /**< Left vector. */
    shvec4 up;        /**< Up vector. */
} ShTransform;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TRANSFORM_H
