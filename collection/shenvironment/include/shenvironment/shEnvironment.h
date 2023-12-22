/**
 * @file shEnvironment.h
 * @brief Handling environment descriptor functionalities in the `shengine` framework.
 *
 * The shEnvironment.h file contains structures and functions related to handling environment descriptors
 * in the `shengine` framework.
 */

#ifndef SH_ENGINE_DESCRIPTOR_H
#define SH_ENGINE_DESCRIPTOR_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <sys/types.h>
#include <sys/stat.h>

#ifdef _UNIX
#define stat _stat
#include <unistd.h>
#endif // _UNIX

#ifdef _WIN32
#define stat _stat
#endif // _WIN32

#include <stdio.h>
#include <stdint.h>
#include <smd/smd.h>
#include <shvulkan/shVulkan.h>



#define SH_ENVIRONMENT_STR512_LENGTH  64
#define SH_ENVIRONMENT_STR1024_LENGTH 128



/**
 * @struct ShIniProperties
 * @brief Structure representing properties related to the initialization (INI) file.
 *
 * The ShIniProperties structure represents properties related to the initialization (INI) file in the `shengine` framework,
 * including application name, and paths to various SMD files.
 */
typedef struct ShIniProperties {
    char application_name       [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Application name. */
    char application_smd_path   [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Path to the application SMD file. */
    char host_memory_smd_path   [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Path to the host memory SMD file. */
    char vulkan_memory_smd_path [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Path to the Vulkan memory SMD file. */
    char serial_smd_path        [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Path to the serial SMD file. */
    char scene_smd_path         [SH_ENVIRONMENT_STR1024_LENGTH]; /**< Path to the scene SMD file. */
} ShIniProperties;



/**
 * @struct ShApplicationProperties
 * @brief Structure representing properties related to the application.
 *
 * The ShApplicationProperties structure represents properties related to the application in the `shengine` framework,
 * including shared name, thread-related function names, and additional thread count.
 */
typedef struct ShApplicationProperties {
    char     shared_name             [SH_ENVIRONMENT_STR512_LENGTH]; /**< Shared name. */
    char     s_start                 [SH_ENVIRONMENT_STR512_LENGTH]; /**< Thread start function name. */
    char     s_update                [SH_ENVIRONMENT_STR512_LENGTH]; /**< Thread update function name. */
    char     s_main_cmd_buffer       [SH_ENVIRONMENT_STR512_LENGTH]; /**< Main command buffer function name. */
    char     s_main_renderpass       [SH_ENVIRONMENT_STR512_LENGTH]; /**< Main render pass function name. */
    char     s_frame_resize          [SH_ENVIRONMENT_STR512_LENGTH]; /**< Frame resize function name. */
    char     s_close                 [SH_ENVIRONMENT_STR512_LENGTH]; /**< Close function name. */
    uint32_t additional_thread_count;                                 /**< Additional thread count. */
} ShApplicationProperties;



#define SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT 2048



/**
 * @struct ShHostMemoryProperties
 * @brief Structure representing properties related to host memory.
 *
 * The ShHostMemoryProperties structure represents properties related to host memory in the `shengine` framework,
 * including buffer count, buffer sizes, buffer strides, buffer offsets, and pointers to buffer memory.
 */
typedef struct ShHostMemoryProperties {
    uint32_t buffer_count;                                         /**< Buffer count. */
    uint32_t buffers_size     [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT]; /**< Buffer sizes. */
    uint32_t buffers_stride   [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT]; /**< Buffer strides. */
    uint32_t buffers_offset   [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT]; /**< Buffer offsets. */
    void*    p_buffers_memory [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT]; /**< Pointers to buffer memory. */
} ShHostMemoryProperties;



#define SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT 512



/**
 * @struct ShVulkanMemoryProperties
 * @brief Structure representing properties related to Vulkan memory.
 *
 * The ShVulkanMemoryProperties structure represents properties related to Vulkan memory in the `shengine` framework,
 * including buffer count, buffer sizes, buffer usages, memory sharing modes, and memory properties.
 */
typedef struct ShVulkanMemoryProperties {
    uint32_t              buffer_count;                                    /**< Buffer count. */
    uint32_t              buffers_size                              [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer sizes. */
    uint8_t               buffers_usage_transfer_src_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for transfer source. */
    uint8_t               buffers_usage_transfer_dst_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for transfer destination. */
    uint8_t               buffers_usage_uniform_buffer_bit          [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for uniform buffer. */
    uint8_t               buffers_usage_storage_buffer_bit          [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for storage buffer. */
    uint8_t               buffers_usage_index_buffer_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for index buffer. */
    uint8_t               buffers_usage_vertex_buffer_bit           [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer usage for vertex buffer. */

    uint8_t               buffers_memory_sharing_mode_exclusive     [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer memory sharing mode (exclusive). */
    uint8_t               buffers_memory_sharing_mode_concurrent    [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer memory sharing mode (concurrent). */

    uint8_t               buffers_memory_property_device_local_bit  [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer memory property (device local). */
    uint8_t               buffers_memory_property_host_visible_bit  [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer memory property (host visible). */
    uint8_t               buffers_memory_property_host_coherent_bit [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Buffer memory property (host coherent). */

    VkBuffer              buffers                                   [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Vulkan buffer objects. */
    VkDeviceMemory        buffers_memory                            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT]; /**< Vulkan buffer memory objects. */
} ShVulkanMemoryProperties;



/**
 * @struct ShSerialProperties
 * @brief Structure representing properties related to serial communication.
 *
 * The ShSerialProperties structure represents properties related to serial communication in the `shengine` framework,
 * including port name, baud rate, read timeout, and read/write bits.
 */
typedef struct ShSerialProperties {
    char     port[SH_ENVIRONMENT_STR1024_LENGTH]; /**< Port name. */
    uint32_t baud_rate;                           /**< Baud rate. */
    uint32_t read_timeout_ms;                     /**< Read timeout in milliseconds. */
    uint8_t  read_bit;                            /**< Read bit. */
    uint8_t  write_bit;                           /**< Write bit. */
} ShSerialProperties;



#include <shcomponents/shIdentity.h>
#include <shcomponents/shCamera.h>
#include <shcomponents/shTransform.h>
//#include <shcomponents/shInputLinker.h>



/**
 * @struct ShSceneProperties
 * @brief Structure representing properties related to the scene.
 *
 * The ShSceneProperties structure represents properties related to the scene in the `shengine` framework,
 * including entity count, and pointers to identity, camera, transform, and host memory linker components.
 */
typedef struct ShSceneProperties {
    uint32_t            entity_count;             /**< Entity count. */
    ShIdentity*         p_identities;             /**< Pointers to identity components. */
    ShCamera*           p_cameras;                /**< Pointers to camera components. */
    ShTransform*        p_transforms;             /**< Pointers to transform components. */
    //ShHostMemoryLinker* p_host_memory_linkers;
} ShSceneProperties;



/**
 * @brief Function to get INI properties from a file.
 *
 * This function retrieves INI properties from the specified INI file and initializes the corresponding structures.
 *
 * @param ini_file_path    Path to the INI file.
 * @param p_ini_smd        Pointer to an `SmdFileHandle` structure for INI properties.
 * @param p_ini_properties Pointer to the ShIniProperties structure to store the retrieved INI properties.
 * @return                 1 on success, 0 on failure.
 */
extern uint8_t shGetIniProperties(
    const char*      ini_file_path,
    SmdFileHandle*   p_ini_smd,
    ShIniProperties* p_ini_properties
);

/**
 * @brief Function to get application properties from a file.
 *
 * This function retrieves application properties from the specified application file and initializes the corresponding structures.
 *
 * @param application_file_path    Path to the application file.
 * @param p_application_smd        Valid pointer to an `SmdFileHandle` structure for application properties.
 * @param p_application_properties Pointer to the ShApplicationProperties structure to store the retrieved application properties.
 * @return                         1 on success, 0 on failure.
 */
extern uint8_t shGetApplicationProperties(
    const char*              application_file_path,
    SmdFileHandle*           p_application_smd,
    ShApplicationProperties* p_application_properties
);

/**
 * @brief Function to get host memory properties from a file.
 *
 * This function retrieves host memory properties from the specified card inputs file and initializes the corresponding structures.
 *
 * @param card_inputs_file_path     Path to the card inputs file.
 * @param p_card_inputs_smd         Pointer to an `SmdFileHandle` structure for host memory properties.
 * @param p_card_inputs_properties  Pointer to the ShHostMemoryProperties structure to store the retrieved host memory properties.
 * @return                          1 on success, 0 on failure.
 */
extern uint8_t shGetHostMemoryProperties(
    const char*             card_inputs_file_path,
    SmdFileHandle*          p_card_inputs_smd,
    ShHostMemoryProperties* p_card_inputs_properties
);

/**
 * @brief Function to get Vulkan memory properties from a file.
 *
 * This function retrieves Vulkan memory properties from the specified Vulkan memory file and initializes the corresponding structures.
 *
 * @param vulkan_memory_file_path       Path to the Vulkan memory file.
 * @param p_vulkan_memory_smd           Pointer to an `SmdFileHandle` structure for Vulkan memory properties.
 * @param p_vulkan_memory_properties    Pointer to the ShVulkanMemoryProperties structure to store the retrieved Vulkan memory properties.
 * @return                              1 on success, 0 on failure.
 */
extern uint8_t shGetVulkanMemoryProperties(
    const char*               vulkan_memory_file_path,
    SmdFileHandle*            p_vulkan_memory_smd,
    ShVulkanMemoryProperties* p_vulkan_memory_properties
);

/**
 * @brief Retrieves serial properties from a specified file.
 *
 * @param serial_file_path      Path to the serial file.
 * @param p_serial_smd          Valid pointer to an `SmdFileHandle` structure for serial.
 * @param p_serial_properties   Valid pointer to an @ref ShSerialProperties structure.
 * 
 * @return 1 on success, 0 on failure.
 */
extern uint8_t shGetSerialProperties(
    const char*         serial_file_path,
    SmdFileHandle*      p_serial_smd,
    ShSerialProperties* p_serial_properties
);

/**
 * @brief Retrieves scene properties from a specified file.
 *
 * @param scene_file_path       Path to the scene file.
 * @param p_scene_smd           Valid pointer to an `SmdFileHandle` structure for the scene.
 * @param p_scene_properties    Valid pointer to an @ref ShSceneProperties structure.
 * 
 * @return 1 on success, 0 on failure.
 */
extern uint8_t shGetSceneProperties(
    const char*        scene_file_path,
    SmdFileHandle*     p_scene_smd,
    ShSceneProperties* p_scene_properties
);

/**
 * @brief Macro to get file statistics.
 *
 * @param path   File path.
 * @param stats  Pointer to a structure to store file statistics.
 */
#define shGetFileStats(path, stats)\
    stat(path, stats)

/**
 * @brief Macro to initialize a descriptor handle.
 *
 * @param p_descriptor_handle  Valid pointer to a descriptor handle.
 */
#define shInitDescriptor(p_descriptor_handle)\
    shGetFileStats((p_descriptor_handle)->path, &(p_descriptor_handle)->stats0)

/**
 * @brief Macro for handling environment errors.
 *
 * @param condition           Condition to check.
 * @param msg                 Error message.
 * @param failure_expression  Expression to execute in case of failure.
 */
#define shEnvironmentError(condition, msg, failure_expression)\
    if ((int)(condition)) { printf("shenvironment error: %s.\n", msg); failure_expression; }



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_DESCRIPTOR_H
