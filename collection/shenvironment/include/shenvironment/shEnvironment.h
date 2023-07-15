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



#define SH_ENVIRONMENT_STR512_LENGTH  64
#define SH_ENVIRONMENT_STR1024_LENGTH 128



typedef struct ShIniProperties {
	char application_name       [SH_ENVIRONMENT_STR1024_LENGTH];
	char application_smd_path   [SH_ENVIRONMENT_STR1024_LENGTH];
	char host_memory_smd_path   [SH_ENVIRONMENT_STR1024_LENGTH];
	char vulkan_memory_smd_path [SH_ENVIRONMENT_STR1024_LENGTH];
	char serial_smd_path        [SH_ENVIRONMENT_STR1024_LENGTH];
	char scene_smd_path         [SH_ENVIRONMENT_STR1024_LENGTH];
} ShIniProperties;



typedef struct ShApplicationProperties {
	char                     shared_name             [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_start                 [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_update                [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_main_cmd_buffer       [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_main_renderpass       [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_frame_resize          [SH_ENVIRONMENT_STR512_LENGTH];
	char                     s_close                 [SH_ENVIRONMENT_STR512_LENGTH];
	uint32_t                 additional_thread_count;
} ShApplicationProperties;



#define SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT 2048

typedef struct ShHostMemoryProperties {
	uint32_t buffer_count;
	uint32_t buffers_size     [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT];
	uint32_t buffers_stride   [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT];
	uint32_t buffers_offset   [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT];
	void*    p_buffers_memory [SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT];
} ShHostMemoryProperties;



#include <shvulkan/shVulkan.h>

#define SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT 512

typedef struct ShVulkanMemoryProperties {
	uint32_t              buffer_count;
	uint32_t              buffers_size                              [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_transfer_src_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_transfer_dst_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_uniform_buffer_bit          [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_storage_buffer_bit          [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_index_buffer_bit            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_usage_vertex_buffer_bit           [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];

	uint8_t               buffers_memory_sharing_mode_exclusive     [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_memory_sharing_mode_concurrent    [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	
	uint8_t               buffers_memory_property_device_local_bit  [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_memory_property_host_visible_bit  [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	uint8_t               buffers_memory_property_host_coherent_bit [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];

	VkBuffer              buffers                                   [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
	VkDeviceMemory        buffers_memory                            [SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT];
} ShVulkanMemoryProperties;



typedef struct ShSerialProperties {
	char     port[SH_ENVIRONMENT_STR1024_LENGTH];
	uint32_t baud_rate;
	uint32_t read_timeout_ms;
	uint8_t  read_bit;
	uint8_t  write_bit;
} ShSerialProperties;



#include <shcomponents/shIdentity.h>
#include <shcomponents/shCamera.h>
#include <shcomponents/shTransform.h>
#include <shcomponents/shInputLinker.h>

typedef struct ShSceneProperties {
	uint32_t            entity_count;
	ShIdentity*         p_identities;
	ShCamera*           p_cameras;
	ShTransform*        p_transforms;
	ShHostMemoryLinker* p_host_memory_linkers;
} ShSceneProperties;



extern uint8_t shGetIniProperties(
	const char*      ini_file_path,
	SmdFileHandle*   p_ini_smd,
	ShIniProperties* p_ini_properties
);

extern uint8_t shGetApplicationProperties(
    const char*              application_file_path,
    SmdFileHandle*           p_application_smd,
    ShApplicationProperties* p_application_properties
);

extern uint8_t shGetHostMemoryProperties(
	const char*             card_inputs_file_path,
	SmdFileHandle*          p_card_inputs_smd,
	ShHostMemoryProperties* p_card_inputs_properties
);

extern uint8_t shGetVulkanMemoryProperties(
	const char*               vulkan_memory_file_path,
	SmdFileHandle*            p_vulkan_memory_smd,
	ShVulkanMemoryProperties* p_vulkan_memory_properties
);

extern uint8_t shGetSerialProperties(
	const char*         serial_file_path,
	SmdFileHandle*      p_serial_smd,
	ShSerialProperties* p_serial_properties
);

extern uint8_t shGetSceneProperties(
	const char*        scene_file_path,
	SmdFileHandle*     p_scene_smd,
	ShSceneProperties* p_scene_properties
);



#define shGetFileStats(path, stats)\
	stat(path, stats)

#define shInitDescriptor(p_descriptor_handle)\
	shGetFileStats((p_descriptor_handle)->path, &(p_descriptor_handle)->stats0)

#define shEnvironmentError(condition, msg, failure_expression)\
	if ((int)(condition)) { printf("shenvironment error: %s.\n", msg); failure_expression; }



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_DESCRIPTOR_H