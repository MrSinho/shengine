/**
 * @file
 * @brief Defines the ShIdentity structure and related constants.
 *
 * The ShIdentity structure represents an identity in the engine, which includes a name, tag, and subtag.
 */

#ifndef SH_IDENTITY_H
#define SH_IDENTITY_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



/** @def SH_IDENTITY_STR256_LENGTH
 * @brief The length of the identity string used in the @ref ShIdentity structure.
 */
#define SH_IDENTITY_STR256_LENGTH 32



/**
 * @struct ShIdentity
 * @brief Represents an identity component in scene.
 */
typedef struct ShIdentity {
    char name[SH_IDENTITY_STR256_LENGTH];    /**< The name of the identity.   */
    char tag[SH_IDENTITY_STR256_LENGTH];     /**< The tag of the identity.    */
    char subtag[SH_IDENTITY_STR256_LENGTH];  /**< The subtag of the identity. */
} ShIdentity;




#ifdef __cplusplus
}
#endif//__cplusplus

#endif // SH_IDENTITY_H
