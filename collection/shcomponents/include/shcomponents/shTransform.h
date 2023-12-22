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



/**
 * @struct ShTransform
 * @brief Represents a transformation in the engine.
 */
typedef struct ShTransform {
    float model[4][4];  /**< Model matrix. */
    float position[4];  /**< Position vector. */
    float rotation[4];  /**< Rotation vector. */
    float euler[4];     /**< Euler vector. */
    float scale[4];     /**< Scale vector. */
    float front[4];     /**< Front vector. */
    float left[4];      /**< Left vector. */
    float up[4];        /**< Up vector. */
} ShTransform;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TRANSFORM_H
