#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shenvironment/shEnvironment.h"

#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>



#ifdef _MSC_VER
#pragma warning (disable: 4996)
#endif//_MSC_VER



uint8_t shGetIniProperties(
    const char*      ini_file_path,
    SmdFileHandle*   p_ini_smd,
    ShIniProperties* p_ini_properties
) {
    shEnvironmentError(ini_file_path    == NULL, "shGetIniProperties: invalid ini file path",         return 0);
    shEnvironmentError(p_ini_properties == NULL, "shGetIniProperties: invalid ini properties memory", return 0);

    p_ini_smd = smdAllocateFileHandle();
    shEnvironmentError(
        p_ini_smd == NULL,
        "shGetIniProperties: invalid ini smd memory",
        return 0
    );

    shEnvironmentError(
        smdReadFile(ini_file_path, p_ini_smd) == 0,
        "shGetIniProperties: failed reading ini file",
        return 0
    );

    shEnvironmentError(
        smdParseMemory(p_ini_smd) == 0,
        "shGetIniProperties: failed parsing memory",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_ini_smd, "ShEngine::ini_properties.application_name", NULL, p_ini_properties->application_name) == 0,
        "shGetIniProperties: failed accessing ShEngine::ini_properties.application_name",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_ini_smd, "ShEngine::ini_properties.application_smd_path", NULL, p_ini_properties->application_smd_path) == 0,
        "shGetIniProperties: failed accessing ShEngine::ini_properties.application_smd_path",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_ini_smd, "ShEngine::ini_properties.host_memory_smd_path", NULL, p_ini_properties->host_memory_smd_path) == 0,
        "shGetIniProperties: failed accessing ShEngine::ini_properties.host_memory_smd_path",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_ini_smd, "ShEngine::ini_properties.vulkan_memory_smd_path", NULL, p_ini_properties->vulkan_memory_smd_path) == 0,
        "shGetIniProperties: failed accessing ShEngine::ini_properties.vulkan_memory_smd_path",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_ini_smd, "ShEngine::ini_properties.scene_smd_path", NULL, p_ini_properties->scene_smd_path) == 0,
        "shGetIniProperties: failed accessing ShEngine::ini_properties.scene_smd_path",
        return 0
    );

    return 1;
}

uint8_t shGetApplicationProperties(
    const char*              application_file_path,
    SmdFileHandle*           p_application_smd,
    ShApplicationProperties* p_application_properties
) {
    shEnvironmentError(application_file_path    == NULL, "shGetApplicationProperties: invalid ini file path",         return 0);
    shEnvironmentError(p_application_properties == NULL, "shGetApplicationProperties: invalid ini properties memory", return 0);
    
    p_application_smd = smdAllocateFileHandle();
    shEnvironmentError(
        p_application_smd == NULL,
        "shGetApplicationProperties: invalid application smd memory",
        return 0
    );

    shEnvironmentError(
        smdReadFile(application_file_path, p_application_smd) == 0,
        "shGetApplicationProperties: failed reading application file",
        return 0
    );

    shEnvironmentError(
        smdParseMemory(p_application_smd) == 0,
        "shGetApplicationProperties: failed parsing memory",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.run", NULL, &p_application_properties->run) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.run",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.shared_path", NULL, p_application_properties->shared_path) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.shared_path",
        return 0
    );
    
    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_start", NULL, p_application_properties->s_start) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_start",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_thread", NULL, p_application_properties->s_thread) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_thread",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_update_pending", NULL, p_application_properties->s_update_pending) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_update_pending",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_after_thread", NULL, p_application_properties->s_after_thread) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_after_thread",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_update", NULL, p_application_properties->s_update) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_update",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_main_renderpass", NULL, p_application_properties->s_main_renderpass) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_main_renderpass",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_frame_resize", NULL, p_application_properties->s_frame_resize) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_frame_resize",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_application_smd, "ShEngine::application_properties.s_close", NULL, p_application_properties->s_close) == 0,
        "shGetApplicationProperties: failed accessing ShEngine::application_properties.s_close",
        return 0
    );

    return 1;
}

uint8_t shGetHostMemoryProperties(
    const char*             host_memory_file_path,
    SmdFileHandle*          p_host_memory_smd,
    ShHostMemoryProperties* p_host_memory_properties
) {
    shEnvironmentError(host_memory_file_path    == NULL, "shGetHostMemoryProperties: invalid host memory file path",         return 0);
    shEnvironmentError(p_host_memory_properties == NULL, "shGetHostMemoryProperties: invalid host memory properties memory", return 0);

    p_host_memory_smd = smdAllocateFileHandle();
    shEnvironmentError(
        p_host_memory_smd == NULL,
        "shGetHostMemoryProperties: invalid host memory smd memory",
        return 0
    );

    shEnvironmentError(
        smdReadFile(host_memory_file_path, p_host_memory_smd) == 0,
        "shGetHostMemoryProperties: failed reading host memory file",
        return 0
    );

    shEnvironmentError(
        smdParseMemory(p_host_memory_smd) == 0,
        "shGetHostMemoryProperties: failed parsing memory",
        return 0
    );

    char s_buffer_count   [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::host_memory_properties.buffer_count";
    
    shEnvironmentError(
        smdAccessVarByName(p_host_memory_smd, s_buffer_count, NULL, &p_host_memory_properties->buffer_count) == 0,
        "shGetHostMemoryProperties: failed accessing ShEngine::host_memory_properties.buffer_count",
        return 0
    );

    shEnvironmentError(
        p_host_memory_properties->buffer_count > SH_ENVIRONMENT_MAX_HOST_BUFFER_COUNT,
        "shGetHostMemoryProperties: invalid buffer count",
        return 0
    );

    for (uint32_t buffer_idx = 0; buffer_idx < p_host_memory_properties->buffer_count; buffer_idx++) {

        char s_buffer_idx[32] = { 0 };
        itoa(buffer_idx, s_buffer_idx, 10);

        char s_buffers_size   [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::host_memory_properties.buffers_size[";  
        char s_buffers_stride [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::host_memory_properties.buffers_stride[";
        char s_buffers_offset [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::host_memory_properties.buffers_offset[";
        char s_buffers_memory [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::host_memory_properties.buffers_memory["; 

        strcat(s_buffers_size,   s_buffer_idx);
        strcat(s_buffers_stride, s_buffer_idx);
        strcat(s_buffers_offset, s_buffer_idx);
        strcat(s_buffers_memory, s_buffer_idx);

        strcat(s_buffers_size,   "]");
        strcat(s_buffers_stride, "]");
        strcat(s_buffers_offset, "]");
        strcat(s_buffers_memory, "]");

        shEnvironmentError(
            smdAccessVarByName(p_host_memory_smd, s_buffers_size, NULL, &p_host_memory_properties->buffers_size[buffer_idx]) == 0,
            "shGetHostMemoryProperties: failed accessing ShEngine::host_memory_properties.buffers_size[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_host_memory_smd, s_buffers_stride, NULL, &p_host_memory_properties->buffers_stride[buffer_idx]) == 0,
            "shGetHostMemoryProperties: failed accessing ShEngine::host_memory_properties.buffers_stride[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_host_memory_smd, s_buffers_offset, NULL, &p_host_memory_properties->buffers_offset[buffer_idx]) == 0,
            "shGetHostMemoryProperties: failed accessing ShEngine::host_memory_properties.buffers_offset[x]",
            return 0
        );

        if (p_host_memory_properties->buffers_size[buffer_idx] != 0) {

            p_host_memory_properties->p_buffers_memory[buffer_idx] = calloc(1, p_host_memory_properties->buffers_size[buffer_idx]);

            shEnvironmentError(
                smdAccessVarByName(p_host_memory_smd, s_buffers_memory, NULL, p_host_memory_properties->p_buffers_memory[buffer_idx]) == 0,
                "shGetHostMemoryProperties: failed accessing ShEngine::host_memory_properties.buffers_memory[x]",
                return 0
            );
        }
        
    }

    return 1;
}

uint8_t shGetVulkanMemoryProperties(
	const char*               vulkan_memory_file_path,
	SmdFileHandle*            p_vulkan_memory_smd,
	ShVulkanMemoryProperties* p_vulkan_memory_properties
) {
    shEnvironmentError(vulkan_memory_file_path    == NULL, "shGetVulkanMemoryProperties: invalid vulkan memory file path",         return 0);
    shEnvironmentError(p_vulkan_memory_properties == NULL, "shGetVulkanMemoryProperties: invalid vulkan memory properties memory", return 0);

    p_vulkan_memory_smd = smdAllocateFileHandle();
    shEnvironmentError(
        p_vulkan_memory_smd == NULL,
        "shGetVulkanMemoryProperties: invalid vulkan memory smd memory",
        return 0
    );

    shEnvironmentError(
        smdReadFile(vulkan_memory_file_path, p_vulkan_memory_smd) == 0,
        "shGetVulkanMemoryProperties: failed reading vulkan memory file",
        return 0
    );

    shEnvironmentError(
        smdParseMemory(p_vulkan_memory_smd) == 0,
        "shGetVulkanMemoryProperties: failed parsing memory",
        return 0
    );

    char s_buffer_count[SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffer_count";

    shEnvironmentError(
        smdAccessVarByName(p_vulkan_memory_smd, s_buffer_count, NULL, &p_vulkan_memory_properties->buffer_count) == 0,
        "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffer_count",
        return 0
    );

    shEnvironmentError(
        p_vulkan_memory_properties->buffer_count > SH_ENVIRONMENT_MAX_VULKAN_BUFFER_COUNT,
        "shGetVulkanMemoryProperties: invalid buffer count",
        return 0
    );

    for (uint32_t buffer_idx = 0; buffer_idx < p_vulkan_memory_properties->buffer_count; buffer_idx++) {

        char s_buffer_idx[32] = { 0 };
        itoa(buffer_idx, s_buffer_idx, 10);

        char s_buffers_size                              [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_size[";
        char s_buffers_usage_transfer_src_bit            [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_usage_transfer_src_bit[";
        char s_buffers_usage_transfer_dst_bit            [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_usage_transfer_dst_bit[";
        char s_buffers_usage_storage_buffer_bit          [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_usage_storage_buffer_bit[";
        char s_buffers_usage_index_buffer_bit            [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_usage_index_buffer_bit[";
        char s_buffers_usage_vertex_buffer_bit           [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_usage_vertex_buffer_bit[";
        char s_buffers_memory_sharing_mode_exclusive     [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_memory_sharing_mode_exclusive[";
        char s_buffers_memory_sharing_mode_concurrent    [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_memory_sharing_mode_concurrent[";
        char s_buffers_memory_property_device_local_bit  [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_memory_property_device_local_bit[";
        char s_buffers_memory_property_host_visible_bit  [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_memory_property_host_visible_bit[";
        char s_buffers_memory_property_host_coherent_bit [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::vulkan_memory_properties.buffers_memory_property_host_coherent_bit[";

        strcat(s_buffers_size,                              s_buffer_idx);
        strcat(s_buffers_usage_transfer_src_bit,            s_buffer_idx);
        strcat(s_buffers_usage_transfer_dst_bit,            s_buffer_idx);
        strcat(s_buffers_usage_storage_buffer_bit,          s_buffer_idx);
        strcat(s_buffers_usage_index_buffer_bit,            s_buffer_idx);
        strcat(s_buffers_usage_vertex_buffer_bit,           s_buffer_idx);
        strcat(s_buffers_memory_sharing_mode_exclusive,     s_buffer_idx);
        strcat(s_buffers_memory_sharing_mode_concurrent,    s_buffer_idx);
        strcat(s_buffers_memory_property_device_local_bit,  s_buffer_idx);
        strcat(s_buffers_memory_property_host_visible_bit,  s_buffer_idx);
        strcat(s_buffers_memory_property_host_coherent_bit, s_buffer_idx);

        strcat(s_buffers_size,                              "]");
        strcat(s_buffers_usage_transfer_src_bit,            "]");
        strcat(s_buffers_usage_transfer_dst_bit,            "]");
        strcat(s_buffers_usage_storage_buffer_bit,          "]");
        strcat(s_buffers_usage_index_buffer_bit,            "]");
        strcat(s_buffers_usage_vertex_buffer_bit,           "]");
        strcat(s_buffers_memory_sharing_mode_exclusive,     "]");
        strcat(s_buffers_memory_sharing_mode_concurrent,    "]");
        strcat(s_buffers_memory_property_device_local_bit,  "]");
        strcat(s_buffers_memory_property_host_visible_bit,  "]");
        strcat(s_buffers_memory_property_host_coherent_bit, "]");

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_size, NULL, &p_vulkan_memory_properties->buffers_size[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_size[x]",
            return 0
        );



        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_usage_transfer_src_bit, NULL, &p_vulkan_memory_properties->buffers_usage_transfer_src_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_usage_transfer_src_bit[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_usage_storage_buffer_bit, NULL, &p_vulkan_memory_properties->buffers_usage_storage_buffer_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_usage_storage_buffer_bit[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_usage_index_buffer_bit, NULL, &p_vulkan_memory_properties->buffers_usage_index_buffer_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_usage_index_buffer_bit[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_usage_vertex_buffer_bit, NULL, &p_vulkan_memory_properties->buffers_usage_vertex_buffer_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_usage_vertex_buffer_bit[x]",
            return 0
        );



        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_memory_sharing_mode_exclusive, NULL, &p_vulkan_memory_properties->buffers_memory_sharing_mode_exclusive[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_memory_sharing_mode_exclusive[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_memory_sharing_mode_concurrent, NULL, &p_vulkan_memory_properties->buffers_memory_sharing_mode_concurrent[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_memory_sharing_mode_concurrent[x]",
            return 0
        );



        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_memory_property_device_local_bit, NULL, &p_vulkan_memory_properties->buffers_memory_property_device_local_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_memory_property_device_local_bit[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_memory_property_host_visible_bit, NULL, &p_vulkan_memory_properties->buffers_memory_property_host_visible_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_memory_property_host_visible_bit[x]",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_vulkan_memory_smd, s_buffers_memory_property_host_coherent_bit, NULL, &p_vulkan_memory_properties->buffers_memory_property_host_coherent_bit[buffer_idx]) == 0,
            "shGetVulkanMemoryProperties: failed accessing ShEngine::vulkan_memory_properties.buffers_memory_property_host_coherent_bit[x]",
            return 0
        );

    }

    return 1;
}

uint8_t shGetSceneProperties(
	const char*        scene_file_path,
	SmdFileHandle*     p_scene_smd,
	ShSceneProperties* p_scene_properties
) {
    shEnvironmentError(scene_file_path    == NULL, "shGetSceneProperties: invalid scene file path",         return 0);
    shEnvironmentError(p_scene_properties == NULL, "shGetSceneProperties: invalid scene properties memory", return 0);

    p_scene_smd = smdAllocateFileHandle();
    shEnvironmentError(
        p_scene_smd == NULL,
        "shGetSceneProperties: invalid scene smd memory",
        return 0
    );

    shEnvironmentError(
        smdReadFile(scene_file_path, p_scene_smd) == 0,
        "shGetSceneProperties: failed reading scene file",
        return 0
    );

    shEnvironmentError(
        smdParseMemory(p_scene_smd) == 0,
        "shGetSceneProperties: failed parsing memory",
        return 0
    );

    shEnvironmentError(
        smdAccessVarByName(p_scene_smd, "ShEngine::scene_properties.entity_count", NULL, &p_scene_properties->entity_count) == 0,
        "shGetSceneProperties: failed accessing ShEngine::scene_properties.entity_count",
        return 0
    );

    if (p_scene_properties->entity_count == 0) {
        return 1;
    }

    //to free
    p_scene_properties->p_identities          = calloc(p_scene_properties->entity_count, sizeof(ShIdentity));
    p_scene_properties->p_cameras             = calloc(p_scene_properties->entity_count, sizeof(ShCamera));
    p_scene_properties->p_transforms          = calloc(p_scene_properties->entity_count, sizeof(ShTransform));
    p_scene_properties->p_host_memory_linkers = calloc(p_scene_properties->entity_count, sizeof(ShHostMemoryLinker));

    shEnvironmentError(
        p_scene_properties->p_identities == NULL,
        "shGetSceneProperties: invalid identities memory",
        return 0
    );

    shEnvironmentError(
        p_scene_properties->p_cameras == NULL,
        "shGetSceneProperties: invalid cameras memory",
        return 0
    );

    shEnvironmentError(
        p_scene_properties->p_transforms == NULL,
        "shGetSceneProperties: invalid transforms memory",
        return 0
    );

    shEnvironmentError(
        p_scene_properties->p_host_memory_linkers == NULL,
        "shGetSceneProperties: invalid host memory linkers memory",
        return 0
    );

    
    for (uint32_t entity = 0; entity < p_scene_properties->entity_count; entity++) {

        char s_entity[32] = { 0 };
        itoa(entity, s_entity, 10);

        //
        //IDENTITY
        //
        char s_identity_name  [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.identities[";
        char s_identity_tag   [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.identities[";
        char s_identity_subtag[SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.identities[";
        
        strcat(s_identity_name,   s_entity);
        strcat(s_identity_tag,    s_entity);
        strcat(s_identity_subtag, s_entity);

        strcat(s_identity_name,   "].name"  );
        strcat(s_identity_tag,    "].tag"   );
        strcat(s_identity_subtag, "].subtag");


        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_identity_name, NULL, p_scene_properties->p_identities[entity].name) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.identities[x].name",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_identity_tag, NULL, p_scene_properties->p_identities[entity].tag) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.identities[x].tag",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_identity_subtag, NULL, p_scene_properties->p_identities[entity].subtag) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.identities[x].subtag",
            return 0
        );

        //
        //CAMERA
        //
        char s_camera_fov         [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";
        char s_camera_nc          [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";
        char s_camera_fc          [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";
        char s_camera_speed       [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";
        char s_camera_mouse_speed [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";
        char s_camera_free_flight [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.cameras[";

        strcat(s_camera_fov,         s_entity);
        strcat(s_camera_nc,          s_entity);
        strcat(s_camera_fc,          s_entity);
        strcat(s_camera_speed,       s_entity);
        strcat(s_camera_mouse_speed, s_entity);
        strcat(s_camera_free_flight, s_entity);

        strcat(s_camera_fov,         "].fov"        );
        strcat(s_camera_nc,          "].nc"         );
        strcat(s_camera_fc,          "].fc"         );
        strcat(s_camera_speed,       "].speed"      );
        strcat(s_camera_mouse_speed, "].mouse_speed");
        strcat(s_camera_free_flight, "].free_flight");

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_fov, NULL, &p_scene_properties->p_cameras[entity].fov) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].fov",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_nc, NULL, &p_scene_properties->p_cameras[entity].nc) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].nc",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_fc, NULL, &p_scene_properties->p_cameras[entity].fc) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].fc",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_speed, NULL, &p_scene_properties->p_cameras[entity].speed) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].speed",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_mouse_speed, NULL, &p_scene_properties->p_cameras[entity].mouse_speed) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].mouse_speed",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_camera_free_flight, NULL, &p_scene_properties->p_cameras[entity].free_flight) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.cameras[x].free_flight",
            return 0
        );



        //
        //TRANSFORM
        //
        char s_transform_position[SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.transforms[";
        char s_transform_rotation[SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.transforms[";
        char s_transform_euler   [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.transforms[";
        char s_transform_scale   [SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.transforms[";
        
        strcat(s_transform_position, s_entity);
        strcat(s_transform_rotation, s_entity);
        strcat(s_transform_euler,    s_entity);
        strcat(s_transform_scale,    s_entity);

        strcat(s_transform_position, "].position");
        strcat(s_transform_rotation, "].rotation");
        strcat(s_transform_euler,    "].euler"   );
        strcat(s_transform_scale,    "].scale"   );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_transform_position, NULL, p_scene_properties->p_transforms[entity].position) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.transforms[x].position",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_transform_rotation, NULL, p_scene_properties->p_transforms[entity].rotation) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.transforms[x].rotation",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_transform_euler, NULL, p_scene_properties->p_transforms[entity].euler) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.transforms[x].euler",
            return 0
        );

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_transform_scale, NULL, p_scene_properties->p_transforms[entity].scale) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.transforms[x].scale",
            return 0
        );

        //
        //HOST MEMORY LINKER
        //
        char s_host_memory_linker_indices[SMD_VAR_NAME_MAX_SIZE] = "ShEngine::scene_properties.host_memory_linkers[";

        strcat(s_host_memory_linker_indices, s_entity);

        strcat(s_host_memory_linker_indices, "].host_memory_indices");

        shEnvironmentError(
            smdAccessVarByName(p_scene_smd, s_host_memory_linker_indices, NULL, p_scene_properties->p_host_memory_linkers[entity].host_memory_indices) == 0,
            "shGetSceneProperties: failed accessing ShEngine::scene_properties.host_memory_linkers[x].host_memory_indices",
            return 0
        );

    }
    

    return 1;
}



#ifdef __cplusplus
}
#endif//__cplusplus
