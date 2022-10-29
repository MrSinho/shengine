#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shfd/shFd.h"
#include "shfd/shFile.h"

#include <shecs/shComponents.h>
#include "../../shengine/include/shlinear-algebra/shEuler.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkCheck.h>
#include <shvulkan/shVkPipelineData.h>
#include <shvulkan/shVkDescriptorStructureMap.h>
    


#ifndef NDEBUG
#include <stdio.h>
#endif

#include <plyimporter/plyImporter.h>

#ifdef _MSC_VER
#pragma warning (disable: 6011 6386 4996)
#endif // _MSC_VER
#include <json.h>



uint8_t shGetIniProperties(const char* loader_ini_dir, ShLoaderIni* p_ini) {
    shFdError(loader_ini_dir == NULL, "invalid loader.ini directory", return 0);
    shFdError(p_ini == NULL, "invalid ini structure memory", return 0);

    FILE* loader_stream = fopen(loader_ini_dir, "r");
    shFdError(loader_stream == NULL, "missing loader.ini", return 0);

    fseek(loader_stream, 0, SEEK_END);
    uint32_t loader_size = ftell(loader_stream);
    fseek(loader_stream, 0, SEEK_SET);
    
    char* p_loader = calloc(1, loader_size);
    shFdError(p_loader == NULL, "invalid loader data", return 0);

    fread(p_loader, 1, loader_size, loader_stream);

    uint32_t simulation_name_size = 0;
    uint32_t assets_path_size     = 0;

    for (uint32_t i = 0; i < loader_size; i++) {
        if (p_loader[i] == '\n') {
            simulation_name_size = i;
            break;
        }
    }

    assets_path_size = loader_size - simulation_name_size - 1;//- 1 for newline
    if (p_loader[strlen(p_loader) - 1] == '\n') {
        assets_path_size--;
    }
    assets_path_size--;//exclude null character

    memcpy(p_ini->simulation_name, p_loader, simulation_name_size);
    memcpy(p_ini->assets_path, &p_loader[simulation_name_size + 1], assets_path_size);

    p_ini->simulation_name[simulation_name_size] = '\0';
    p_ini->assets_path[assets_path_size] = '\0';

    fclose(loader_stream);
    free(p_loader);

    return 1;
}

uint8_t shAppendAssetsPath(const char* engine_assets_path, const char* extension_dir, const char* extension_filename, ShFd* p_fd) {
    shFdError(engine_assets_path == NULL, "invalid engine assets path",                 return 0);
    shFdError(p_fd == NULL,               "invalid destination file descriptor memory", return 0);
    
    //strcpy(dst_path, SH_EDITOR_ASSETS_PATH); //CMake defined macro
    strcpy(p_fd->dir, engine_assets_path);
    if (extension_dir != NULL) {
        strcat(p_fd->dir, extension_dir);
    }

    strcpy(p_fd->filename, extension_filename);

    strcpy(p_fd->path, p_fd->dir);
    strcat(p_fd->path, "/");
    if (extension_filename != NULL) {
        strcat(p_fd->path, p_fd->filename);
    }

    return 1;
}


#define shAbortLoadingMaterials(pp_materials)\
    *(pp_materials) = NULL;\
    return 0

uint8_t shLoadMaterials(ShVkCore* p_core, const char* dir, const char* filename, uint32_t* p_material_count, ShMaterialHost** pp_materials) {
    shFdError(p_material_count == NULL || pp_materials == NULL, "invalid arguments", return 0);
    
    char path[256];
    strcpy(path, dir);
    strcat(path, filename);

    char* buffer = (char*)shReadText(path, NULL);
    shFdError(buffer == NULL, "invalid materials path", shAbortLoadingMaterials(pp_materials));

    json_object* parser = json_tokener_parse(buffer);
    shFdError(parser == NULL, "invalid json format", shAbortLoadingMaterials(pp_materials));
    free(buffer);

    json_object* json_materials = json_object_object_get(parser, "materials");
    shFdError(json_materials == NULL, "missing materials descriptor", shAbortLoadingMaterials(pp_materials));

    uint32_t mat_count              = (p_material_count  == NULL || (*p_material_count) == 0)       ? (uint32_t)json_object_array_length(json_materials)    : (*p_material_count);
    ShMaterialHost* p_materials     = (pp_materials      == NULL || (*pp_materials) == NULL) ? calloc(mat_count, sizeof(ShMaterialHost)) : (*pp_materials);

    if (p_materials == NULL || mat_count == 0) { shAbortLoadingMaterials(pp_materials); }

    for (uint32_t i = 0; i < mat_count; i++) {

        json_object* json_material = json_object_array_get_idx(json_materials, i);


        json_object* json_build_pipeline = json_object_object_get(json_material, "build_pipeline");
        uint8_t build_pipeline = 1;
        if (json_build_pipeline != NULL) {
            build_pipeline = (uint8_t)json_object_get_int(json_build_pipeline);
        }

        for (uint8_t build_idx = (uint8_t)1 - build_pipeline; build_idx < 2; build_idx++) {


            ShVkPipeline pipeline = { 0 };//USED ONLY WHEN build_pipeline == 1



            json_object* json_vertex_shader = json_object_object_get(json_material, "vertex_shader");
            json_object* json_fragment_shader = json_object_object_get(json_material, "fragment_shader");

            {//++++++++++++++++++++++++
                json_object* json_bind_on_loop = json_object_object_get(json_material, "bind_on_loop");
                p_materials[i].bind_on_loop = json_bind_on_loop == NULL ? 1 : (uint8_t)json_object_get_int(json_bind_on_loop);
            }//++++++++++++++++++++++++

            //+++++++++++++++++++++++++        
            {
                ShFd vertex_fd = { 0 };
                ShFd fragment_fd = { 0 };
                uint32_t vertex_shader_size = 0;
                uint32_t fragment_shader_size = 0;

                char shaders_dir[256];
                strcpy(shaders_dir, dir);
                strcat(shaders_dir, "../");

                shFdError(json_vertex_shader == NULL, "missing vertex shader path", shAbortLoadingMaterials(pp_materials));
                shFdError(json_fragment_shader == NULL, "missing fragment shader path", shAbortLoadingMaterials(pp_materials));

                shAppendAssetsPath(shaders_dir, NULL, json_object_get_string(json_vertex_shader), &vertex_fd);
                shAppendAssetsPath(shaders_dir, NULL, json_object_get_string(json_fragment_shader), &fragment_fd);
                char* vertex_code = (char*)shReadBinary(
                    vertex_fd.path,
                    &vertex_shader_size
                );
                char* fragment_code = (char*)shReadBinary(
                    fragment_fd.path,
                    &fragment_shader_size
                );
                shFdError(vertex_code == NULL, "vertex shader not found",     shAbortLoadingMaterials(pp_materials));
                shFdError(fragment_code == NULL, "fragment shader not found", shAbortLoadingMaterials(pp_materials));

                {//BUILD PIPELINE
                    if (build_idx && build_pipeline) {
                        shPipelineCreateShaderModule(p_core->device, vertex_shader_size, vertex_code, &pipeline);
                        shPipelineCreateShaderStage(p_core->device, VK_SHADER_STAGE_VERTEX_BIT, &pipeline);
                        shPipelineCreateShaderModule(p_core->device, fragment_shader_size, fragment_code, &pipeline);
                        shPipelineCreateShaderStage(p_core->device, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline);
                    }
                }//BUILD PIPELINE

                free(vertex_code);
                free(fragment_code);
            }
            //+++++++++++++++++++++++++        


            json_object* json_push_constant_size = json_object_object_get(json_material, "push_constants_size");
            json_object* json_push_constants_stage = json_object_object_get(json_material, "push_constants_stage");
            //+++++++++++++++++++++++++        
            {
                if (json_push_constant_size && json_push_constants_stage) {
                    {//BUILD PIPELINE
                        if (build_idx && build_pipeline) {
                            uint32_t push_constant_size = (uint32_t)json_object_get_int(json_push_constant_size);
                            ShShaderStageFlags push_constant_stage = shStringFlagToInt(json_object_get_string(json_push_constants_stage));
                            shSetPushConstants(push_constant_stage, 0, push_constant_size, &pipeline.push_constant_range);
                        }
                    }//BUILD PIPELINE
                }
            }
            //+++++++++++++++++++++++++        

            json_object* json_descriptors = json_object_object_get(json_material, "descriptors");
            //+++++++++++++++++++++++++
            {
                if (json_descriptors != NULL) {
                    const uint8_t descriptor_buffer_count = (uint8_t)json_object_array_length(json_descriptors);
                    for (uint8_t i = 0; i < descriptor_buffer_count; i++) {
                        json_object* json_descriptor = json_object_array_get_idx(json_descriptors, i);
                        json_object* json_set = json_object_object_get(json_descriptor, "set");
                        json_object* json_max_bindings = json_object_object_get(json_descriptor, "max_bindings");
                        json_object* json_size = json_object_object_get(json_descriptor, "size");
                        json_object* json_stage = json_object_object_get(json_descriptor, "stage");
                        json_object* json_type = json_object_object_get(json_descriptor, "type");

                        shFdError(json_set == NULL || json_size == NULL || json_stage == NULL || json_type == NULL || json_stage == NULL,
                                    "insufficient descriptor set info",
                                    shAbortLoadingMaterials(pp_materials)
                        );

                        {//BUILD PIPELINE
                            if (build_idx && build_pipeline) {
                                uint32_t set = (uint32_t)json_object_get_int(json_set);
                                uint32_t max_bindings = (json_max_bindings == NULL) ? 1 : (uint32_t)json_object_get_int(json_max_bindings);
                                uint32_t size = (uint32_t)json_object_get_int(json_size);
                                const char* s_type = json_object_get_string(json_type);
                                const char* s_stage = json_object_get_string(json_stage);

                                shFdError(s_type == NULL, "missing descriptor type definition", shAbortLoadingMaterials(pp_materials));
                                shFdError(s_stage == NULL, "missing descriptor stage definition", shAbortLoadingMaterials(pp_materials));

                                VkDescriptorType type = (VkDescriptorType)shStringFlagToInt(s_type);
                                VkShaderStageFlags stage = (VkShaderStageFlags)shStringFlagToInt(s_stage);

                                uint8_t dynamic = 0;
                                if (type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC || type == VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC) {
                                    dynamic++;
                                    shPipelineCreateDynamicDescriptorBuffer(
                                        p_core->device,
                                        (type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC) ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                        set,
                                        shGetDescriptorSize(p_core, size),
                                        max_bindings,
                                        &pipeline
                                    );
                                }
                                else {
                                    shPipelineCreateDescriptorBuffer(
                                        p_core->device,
                                        (type == VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER) ? VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT : VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
                                        set,
                                        shGetDescriptorSize(p_core, size),
                                        &pipeline
                                    );
                                }
                                shPipelineAllocateDescriptorBufferMemory(p_core->device, p_core->physical_device, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT | VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, set, &pipeline);
                                shPipelineBindDescriptorBufferMemory(p_core->device, set, 0, &pipeline);
                                shPipelineDescriptorSetLayout(p_core->device,
                                    set,
                                    dynamic,
                                    type,
                                    stage,
                                    &pipeline
                                );
                                shPipelineCreateDescriptorPool(p_core->device, set, &pipeline);
                                shPipelineAllocateDescriptorSet(p_core->device, set, &pipeline);
                            }
                        }//BUILD PIPELINE
                    }//END DESCRIPTORS LOOP
                }
            }
            //+++++++++++++++++++++++++        

    //FIXED STATES
            ShVkFixedStates fixed_states = { 0 };//USED ONLY WHEN build_pipeline == 1
            //FIXED STATES

            json_object* json_fixed_states = json_object_object_get(json_material, "fixed_states");
            json_object* json_primitive_topology = json_object_object_get(json_fixed_states, "primitive_topology");
            json_object* json_polygon_mode = json_object_object_get(json_fixed_states, "polygon_mode");
            //+++++++++++++++++++++++++        
            {//FIXED STATES
                shFdError(json_fixed_states       == NULL, "invalid fixed states info",     shAbortLoadingMaterials(pp_materials));
                shFdError(json_primitive_topology == NULL, "incomplete primitive topology", shAbortLoadingMaterials(pp_materials));
                shFdError(json_polygon_mode       == NULL, "incomplete polygon mode ",      shAbortLoadingMaterials(pp_materials));
                
                json_object* json_vertex_inputs = json_object_object_get(json_fixed_states, "vertex_inputs");
                if (json_vertex_inputs != NULL) {
                    for (uint32_t i = 0; i < (uint32_t)json_object_array_length(json_vertex_inputs); i++) {
                        json_object* json_vertex_input = json_object_array_get_idx(json_vertex_inputs, i);
                        json_object* json_location = json_object_object_get(json_vertex_input, "location");
                        json_object* json_format = json_object_object_get(json_vertex_input, "format");
                        json_object* json_offset = json_object_object_get(json_vertex_input, "offset");
                        json_object* json_size = json_object_object_get(json_vertex_input, "size");
                        shFdError(json_vertex_input == NULL || json_location == NULL || json_format == NULL || json_offset == NULL || json_size == NULL, 
                            "insufficient vertex input state info", 
                            shAbortLoadingMaterials(pp_materials)
                        );

                        {//BUILD PIPELINE
                            if (build_idx && build_pipeline) {
                                shSetVertexInputAttribute(
                                    (uint32_t)json_object_get_int(json_location),
                                    shStringFlagToInt(json_object_get_string(json_format)),
                                    (uint32_t)json_object_get_int(json_offset),
                                    (uint32_t)json_object_get_int(json_size),
                                    &fixed_states
                                );
                            }
                        }//BUILD PIPELINE 
                    }

                }//END VERTEX INPUTS LOOP

                {
                    json_object* json_input_rate = json_object_object_get(json_fixed_states, "vertex_input_rate");
                    shFdError(json_input_rate == NULL && json_vertex_inputs != NULL, "missing input rate specification", shAbortLoadingMaterials(pp_materials));
                    
                    if (build_idx && build_pipeline) {//BUILD PIPELINE
                        if (json_input_rate) {
                            VkVertexInputRate input_rate = shStringFlagToInt(json_object_get_string(json_input_rate));
                            shFixedStatesSetVertexInputRate(input_rate, 0, &fixed_states);
                            shFixedStatesSetVertexInputState(&fixed_states);
                        }
                    }//BUILD PIPELINE
                }//FIXED STATES


                if (build_idx && build_pipeline) {
                    const char* s_polygon_mode = json_object_get_string(json_polygon_mode);
                    const char* s_primitive_topology = json_object_get_string(json_primitive_topology);
                    VkPolygonMode polygon_mode = (VkPolygonMode)shStringFlagToInt(s_polygon_mode);
                    VkPrimitiveTopology primitive_topology = (VkPrimitiveTopology)shStringFlagToInt(s_primitive_topology);
                    shSetFixedStates(
                        p_core->device,
                        p_core->surface.width,
                        p_core->surface.height,
                        primitive_topology,
                        polygon_mode,
                        &fixed_states
                    );
                    shSetupGraphicsPipeline(p_core->device, p_core->render_pass, fixed_states, &pipeline);
                    p_materials[i].fixed_states = fixed_states;
                    p_materials[i].pipeline = pipeline;
                    for (uint32_t j = 0; j < p_materials[i].pipeline.descriptor_count; j++) {
                        p_materials[i].pipeline.write_descriptor_sets[j].pBufferInfo = &p_materials[i].pipeline.descriptor_buffer_infos[j];
                    }
                }
            }//FIXED STATES
        }//END BUILD PIPELINE CHECKS

        (pp_materials       != NULL) && (*pp_materials = p_materials);
        (p_material_count   != NULL) && (*p_material_count = mat_count);

    }//END MATERIALS LOOP
    
    free(parser);
    
    return 1;
}

#ifdef _MSC_VER
#pragma warning (disable: 6001)
#endif//_MSC_VER

void shMaterialsRelease(ShVkCore* p_core, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials) {
	shFdError(p_mat_info_count == NULL || pp_materials == NULL, "invalid arguments", return);
	for (uint32_t i = 0; i < *p_mat_info_count; i++) {
        ShMaterialHost* p_material = &(*pp_materials)[i];
        if (p_material->p_material_clients != NULL) {
            free(p_material->p_material_clients);
        }
        if (p_material->p_entities != NULL) {
            free(p_material->p_entities);
        }
		for (uint32_t j = 0; j < p_material->pipeline.descriptor_count; j++) {
			shPipelineClearDescriptorBufferMemory(p_core->device, j, &p_material->pipeline);
		}
        shPipelineRelease(p_core->device, &(*pp_materials)[i].pipeline);
    }
    if (*p_mat_info_count != 0 && *pp_materials != NULL) {
        free(*pp_materials); 
        (*pp_materials) = NULL;
    }
    *p_mat_info_count = 0;
}

void shReadUniformParameters(json_object* json_parameters, const uint32_t entity, const uint32_t descriptor_idx, ShMaterialHost* p_material) {
    shFdError(json_parameters == NULL || p_material == NULL, "invalid arguments", return);

    uint32_t descriptor_offset = shGetUniformOffset(p_material, descriptor_idx);

    for (uint32_t j = 0; j < (uint32_t)json_object_array_length(json_parameters); j += 2) {//FOR EACH UNIFORM PARAMETER VALUE
        const char* s_type = json_object_get_string(json_object_array_get_idx(json_parameters, j));

        if (strcmp(s_type, "float") == 0) {
            float value = (float)json_object_get_double(json_object_array_get_idx(json_parameters, j + 1));
            memcpy((void*)&((char*)p_material->p_material_clients[entity].p_uniform_parameters)[descriptor_offset], &value, 4);
        }
        if (strcmp(s_type, "int") == 0) {
            int value = (uint32_t)json_object_get_int(json_object_array_get_idx(json_parameters, j + 1));
            memcpy((void*)&((char*)p_material->p_material_clients[entity].p_uniform_parameters)[descriptor_offset], &value, 4);
        }
        if (strcmp(s_type, "transform") == 0) { //EXTENSION STRUCTURES
            p_material->extensions.transform_uniform_offset = descriptor_offset;
            descriptor_offset += 64;
        }
        descriptor_offset += 4;
    }
}

#define shAbortLoadingScene()\
    return 0

uint8_t shLoadScene(const char* dir, const char* filename, const uint32_t material_count, ShMaterialHost** pp_materials, ShScene* p_scene) {
    shFdError(p_scene == NULL || pp_materials == NULL, "invalid arguments", return 0);

    char path[256];
    strcpy(path, dir);
    strcat(path, filename);

    char* buffer = (char*)shReadText(path, NULL);
    shFdError(buffer == NULL, "unavailable scene descriptor", shAbortLoadingScene());

    json_object* parser = json_tokener_parse(buffer);
    free(buffer);
    shFdError(parser == NULL, "invalid json format", shAbortLoadingScene());
    
    //MESHES
    uint32_t ply_mesh_count = 0;
    PlyFileData* ply_meshes = NULL;
    json_object* json_meshes = json_object_object_get(parser, "meshes");
    if (json_meshes) {

        char meshes_dir[256];
        strcpy(meshes_dir, dir);
        strcat(meshes_dir, "../");

        uint32_t mesh_info_count = (uint32_t)json_object_array_length(json_meshes);
        for (uint32_t i = 0; i < mesh_info_count; i++) {
            json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
            json_object_object_get(json_mesh, "path") != NULL && ply_mesh_count++;
        }
        ply_meshes = calloc(ply_mesh_count, sizeof(PlyFileData));
        for (uint32_t i = 0; i < ply_mesh_count; i++) {
            json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
            ShFd mesh_fd = { 0 };
            json_object* json_path = json_object_object_get(json_mesh, "path");
            if (json_path != NULL) {
                shAppendAssetsPath(meshes_dir, NULL, json_object_get_string(json_path), &mesh_fd);
                plyLoadFile(mesh_fd.path, &ply_meshes[i], 0);
            }
        }
    }

    //ENTITIES
    json_object* json_entities = json_object_object_get(parser, "entities");
    json_object* json_max_entities = json_object_object_get(parser, "max_entities");
    json_object* json_max_components = json_object_object_get(parser, "max_components");
    
    shFdError(json_entities == NULL, "missing entities", shAbortLoadingScene());

    uint32_t entity_count   = (uint32_t)json_object_array_length(json_entities);
    uint32_t max_entities   = entity_count * 3;
    uint32_t max_components = 16;

    if (json_max_entities != NULL) {
        max_entities = (uint32_t)json_object_get_int(json_max_entities);
    }
    
    if (json_max_components != NULL) {
        max_components = (uint32_t)json_object_get_int(json_max_components);
    }

    shCreateScene(p_scene, max_entities, max_components);

    for (uint32_t i = 0; i < material_count; i++) {
        ShMaterialHost* p_material = &(*pp_materials)[i];
        if (p_material != NULL) {
            p_material->p_entities = calloc(max_entities, 4);
            p_material->p_material_clients = calloc(max_components, sizeof(ShMaterialClient));
        }
    }

    for (uint32_t i = 0; i < entity_count; i++) {
        uint32_t entity = shCreateEntity(p_scene);
        json_object* json_entity = json_object_array_get_idx(json_entities, i);

        json_object* json_transform         = json_object_object_get(json_entity, "transform");
        json_object* json_mesh              = json_object_object_get(json_entity, "mesh");
        json_object* json_camera            = json_object_object_get(json_entity, "camera");
        json_object* json_material          = json_object_object_get(json_entity, "material");
        json_object* json_identity          = json_object_object_get(json_entity, "identity");
        json_object* json_molecule          = json_object_object_get(json_entity, "molecule");

        if (json_transform != NULL) {
            json_object* json_position = json_object_object_get(json_transform, "position");
            float position[3] = { 0.0f };
            if (json_position != NULL) {
                for (uint32_t j = 0; j < 3; j++) {
                    json_object* json_pos = json_object_array_get_idx(json_position, j);
                    position[j] = json_pos != NULL ? (float)json_object_get_double(json_pos) : 0.0f;
                }
            }
            json_object* json_rotation = json_object_object_get(json_transform, "rotation");
            float rotation[3] = { 0.0f };
            if (json_rotation != NULL) {
                for (uint32_t j = 0; j < 3; j++) {
                    json_object* json_rot = json_object_array_get_idx(json_rotation, j);
                    rotation[j] = json_rot != NULL ? (float)json_object_get_double(json_rot) : 0.0f;
                }
            }
            json_object* json_euler = json_object_object_get(json_transform, "euler");
            if (json_euler != NULL) {
                for (uint32_t j = 0; j < 3; j++) {
                    json_object* json_eu = json_object_array_get_idx(json_euler, j);
                    rotation[j] = json_eu != NULL ? SH_DEGREES_TO_RADIANS((float)json_object_get_double(json_eu)) : 0.0f;
                }
            }
            json_object* json_scale = json_object_object_get(json_transform, "scale");
            float scale[3] = { 1.0f, 1.0f, 1.0f };
            if (json_scale != NULL) {
                for (uint32_t j = 0; j < 3; j++) {
                    json_object* json_scl = json_object_array_get_idx(json_scale, j);
                    scale[j] = json_scl != NULL ? (float)json_object_get_double(json_scl) : 0.0f;
                }
            }
            json_object* json_model = json_object_object_get(json_transform, "model");
            float model[16] = { 0.0f };
            if (json_model != NULL) {
                for (uint32_t j = 0; j < 16; j++) {
                    json_object* json_mdl = json_object_array_get_idx(json_model, j);
                    model[j] = json_mdl != NULL ? (float)json_object_get_double(json_mdl) : 0.0f;
                }
            }
            ShTransform* p_transform = shAddShTransform(p_scene, entity);
            memcpy(p_transform->model, model, 64);
            memcpy(p_transform->position, position, 12);
            memcpy(p_transform->scale, scale, 12);
            memcpy(p_transform->rotation, rotation, 12);
        }
        if (json_mesh != NULL) {
            ShMesh* p_mesh_info = shAddShMesh(p_scene, entity);
            json_object* json_data_index = json_object_object_get(json_mesh, "index");
            if (json_data_index != NULL) {
                uint32_t data_index = json_object_get_int(json_data_index);
                if (data_index < ply_mesh_count) {
                    p_mesh_info->mesh_info.vertex_count = ply_meshes[data_index].vertex_count;
                    p_mesh_info->mesh_info.vertex_stride = ply_meshes[data_index].vertex_stride;
                    p_mesh_info->mesh_info.p_vertices = ply_meshes[data_index].p_vertices;
                    p_mesh_info->mesh_info.index_count = ply_meshes[data_index].index_count;
                    p_mesh_info->mesh_info.p_indices = ply_meshes[data_index].p_indices;
                    json_object* json_flags = json_object_object_get(json_mesh, "flags");
                    p_mesh_info->mesh_info.flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_MESH_SETUP_STATIC_MESH;
                }
            }
        }
        if (json_camera != NULL) {
            ShCamera* p_camera              = shAddShCamera(p_scene, entity);
            json_object* json_fov           = json_object_object_get(json_camera, "fov");
            json_object* json_nc            = json_object_object_get(json_camera, "nc");
            json_object* json_fc            = json_object_object_get(json_camera, "fc");
            json_object* json_speed         = json_object_object_get(json_camera, "speed");
            json_object* json_mouse_speed  = json_object_object_get(json_camera, "mouse_speed");
            json_object* json_flags         = json_object_object_get(json_camera, "flags");
            p_camera->fov = (json_fov == NULL) ? 45.0f : (float)json_object_get_double(json_fov);
            p_camera->nc = (json_nc == NULL) ? 0.001f : (float)json_object_get_double(json_nc);
            p_camera->fc = (json_fc == NULL) ? 1000.0f : (float)json_object_get_double(json_fc);
            p_camera->speed = (json_speed == NULL) ? 15.0f : (float)json_object_get_double(json_speed);
            p_camera->mouse_speed = (json_mouse_speed == NULL) ? 3.0f : (float)json_object_get_double(json_mouse_speed);
            p_camera->flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_CAMERA_SETUP_FREE_FLIGHT;
        }
        if (json_material != NULL) {
            json_object* json_idx = json_object_object_get(json_material, "index");
            if (json_idx != NULL) {
                const uint32_t idx = (uint32_t)json_object_get_int(json_idx);
                ShMaterialHost* p_material = &(*pp_materials)[idx];
                p_material->p_entities[p_material->entity_count] = entity;

                json_object* json_descriptor_parameters     = json_object_object_get(json_material, "uniform_parameters");
                if (json_descriptor_parameters != NULL) {
                    for (uint32_t descriptor_idx = 0; descriptor_idx < json_object_array_length(json_descriptor_parameters); descriptor_idx++) {//for each descriptor
                        shReadUniformParameters(json_object_array_get_idx(json_descriptor_parameters, descriptor_idx), entity, descriptor_idx, p_material);
                        p_material->p_material_clients[entity].parameters = 1;
                    }
                }

                p_material->entity_count++;
            }
           
        }
        if (json_identity != NULL) {
            ShIdentity*  p_identity  = shAddShIdentity(p_scene, entity);
            json_object* json_name   = json_object_object_get(json_identity, "name");
            json_object* json_tag    = json_object_object_get(json_identity, "tag");
            json_object* json_subtag = json_object_object_get(json_identity, "subtag");
            (json_name   != NULL) && (p_identity->name = json_object_get_string(json_name));
            (json_tag    != NULL) && (p_identity->tag = json_object_get_string(json_tag));
            (json_subtag != NULL) && (p_identity->subtag = json_object_get_string(json_subtag));
        }
    }


    //Check entities in materials
    for (uint32_t material_idx = 0; material_idx < material_count; material_idx++) {
        ShMaterialHost* p_material = &(*pp_materials)[material_idx];
        shFdError(p_material->entity_count == 0 && p_material->bind_on_loop, 
            "material is linked to no entities but set to be used in the engine loop", 
            shAbortLoadingScene()
        );
    }
    
    free(parser);
    if (ply_meshes != NULL) { free(ply_meshes); }

    return 1;
}

uint32_t shStringFlagToInt(const char* s_flag) {
    if (strcmp(s_flag, "SHADER_STAGE_VERTEX") == 0) {
        return SH_SHADER_STAGE_VERTEX;
    }
    if (strcmp(s_flag, "SHADER_STAGE_FRAGMENT") == 0) {
        return SH_SHADER_STAGE_FRAGMENT;
    }
    if (strcmp(s_flag, "VEC1_SIGNED_FLOAT") == 0) {
        return SH_VEC1_SIGNED_FLOAT;
    }
    if (strcmp(s_flag, "VEC2_SIGNED_FLOAT") == 0) {
        return SH_VEC2_SIGNED_FLOAT;
    }
    if (strcmp(s_flag, "VEC3_SIGNED_FLOAT") == 0) {
        return SH_VEC3_SIGNED_FLOAT;
    }
    if (strcmp(s_flag, "VEC1_SIGNED_INT") == 0) {
        return SH_VEC1_SIGNED_INT;
    }
    if (strcmp(s_flag, "VEC2_SIGNED_INT") == 0) {
        return SH_VEC2_SIGNED_INT;
    }
    if (strcmp(s_flag, "VEC3_SIGNED_INT") == 0) {
        return SH_VEC3_SIGNED_INT;
    }
    if (strcmp(s_flag, "VEC1_UNSIGNED_INT") == 0) {
        return SH_VEC1_UNSIGNED_INT;
    }
    if (strcmp(s_flag, "VEC2_UNSIGNED_INT") == 0) {
        return SH_VEC2_UNSIGNED_INT;
    }
    if (strcmp(s_flag, "VEC3_UNSIGNED_INT") == 0) {
        return SH_VEC3_UNSIGNED_INT;
    }
    if (strcmp(s_flag, "WIREFRAME") == 0 || strcmp(s_flag, "LINE") == 0) {
        return VK_POLYGON_MODE_LINE;
    }
    if (strcmp(s_flag, "FACE") == 0) {
        return VK_POLYGON_MODE_FILL;
    }
    if (strcmp(s_flag, "POINT") == 0) {
        return VK_POLYGON_MODE_POINT;
    }
    if (strcmp(s_flag, "TRIANGLE_LIST") == 0) {
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    if (strcmp(s_flag, "TRIANGLE_FAN") == 0) {
        return VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN;
    }
    if (strcmp(s_flag, "LINE_LIST") == 0) {
        return VK_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }
    if (strcmp(s_flag, "POINT_LIST") == 0) {
        return VK_PRIMITIVE_TOPOLOGY_POINT_LIST;
    }
    if (strcmp(s_flag, "VERTEX_INPUT_RATE_VERTEX") == 0) {
        return VK_VERTEX_INPUT_RATE_VERTEX;
    }
    if (strcmp(s_flag, "VERTEX_INPUT_RATE_INSTANCE") == 0) {
        return VK_VERTEX_INPUT_RATE_INSTANCE;
    }
    if (strcmp(s_flag, "UNIFORM") == 0) {
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    }
    if (strcmp(s_flag, "STORAGE") == 0) {
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER;
    }
    if (strcmp(s_flag, "DYNAMIC_UNIFORM") == 0) {
        return VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC;
    }
    if (strcmp(s_flag, "DYNAMIC_STORAGE") == 0) {
        return VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_FREE_FLIGHT") == 0) {
        return SH_CAMERA_SETUP_FREE_FLIGHT;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_STATIC") == 0) {
        return SH_CAMERA_SETUP_STATIC;
    }
    if (strcmp(s_flag, "MESH_SETUP_STATIC_MESH") == 0) {
        return SH_MESH_SETUP_STATIC_MESH;
    }
    if (strcmp(s_flag, "MESH_SETUP_DYNAMIC_MESH") == 0) {
        return SH_MESH_SETUP_DYNAMIC_MESH;
    }
#if 0
    if (strcmp(s_flag, "PHYSICS_CLIENT_DYNAMICS") == 0) {
        return SH_PHYSICS_CLIENT_DYNAMICS;
    }
    if (strcmp(s_flag, "PHYSICS_CLIENT_THERMODYNAMICS") == 0) {
        return SH_PHYSICS_CLIENT_THERMODYNAMICS;
    }
    if (strcmp(s_flag, "PHYSICS_CLIENT_ELECTROSTATICS") == 0) {
        return SH_PHYSICS_CLIENT_ELECTROSTATICS;
    }
    if (strcmp(s_flag, "COLLISION_SHAPE_SPHERE") == 0) {
        return SH_COLLISION_SHAPE_SPHERE;
    }
    if (strcmp(s_flag, "COLLISION_SHAPE_POINT") == 0) {
        return SH_COLLISION_SHAPE_BOX;
    }
    if (strcmp(s_flag, "COLLISION_SHAPE_POINT") == 0) {
        return SH_COLLISION_SHAPE_BOX;
    }
    if (strcmp(s_flag, "DYNAMICS_WORLD_GRAVITY") == 0) {
        return SH_DYNAMICS_WORLD_GRAVITY;
    }
    if (strcmp(s_flag, "DYNAMICS_WORLD_NEWTON_3RD_LAW") == 0) {
        return SH_DYNAMICS_WORLD_NEWTON_3RD_LAW;
    }
#endif
    return 0;
}



#ifdef __cplusplus
}
#endif//__cplusplus
