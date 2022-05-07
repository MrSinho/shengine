#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shfd/shFd.h"
#include "shfd/shFile.h"

#include "shecs/shCamera.h"
#include "shecs/shTransform.h"
#include "shecs/shIdentity.h"
#include "shecs/shPhysics.h"
#include "shecs/shMaterial.h"
#include "shecs/shMesh.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkCheck.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

#include <plyimporter/plyImporter.h>

#ifdef _MSC_VER
#pragma warning (disable: 26451 6386 4996)
#endif // _MSC_VER
#include <json.h>

void shMakeAssetsPath(const char* src_path, char* dst_path) {
    strcpy(dst_path, SH_EDITOR_ASSETS_PATH); //CMake defined macro
    strcat(dst_path, "/");
    strcat(dst_path, src_path);
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
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_WIREFRAME") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_WIREFRAME;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_FACE") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_FACE;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_POINTS") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_POINTS;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_TRIANGLE_LIST") == 0) {
        return SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_LINE_LIST") == 0) {
        return SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_POINT_LIST") == 0) {
        return SH_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST;
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
    if (strcmp(s_flag, "PHYSICS_CLIENT_DYNAMICS") == 0) {
        return SH_PHYSICS_CLIENT_DYNAMICS;
    }
    if (strcmp(s_flag, "PHYSICS_CLIENT_THERMODYNAMICS") == 0) {
        return SH_PHYSICS_CLIENT_THERMODYNAMICS;
    }
    if (strcmp(s_flag, "PHYSICS_CLIENT_ELECTROSTATICS") == 0) {
        return SH_PHYSICS_CLIENT_ELECTROSTATICS;
    }
#if 0
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

void shLoadMaterials(ShVkCore* p_core, const char* path, uint32_t* p_material_count, ShMaterialHost** pp_materials) {
    assert(p_material_count != NULL && pp_materials != NULL);

    char* buffer = (char*)shReadText(path, NULL);
    if (buffer == NULL) { return; }

    json_object* parser = json_tokener_parse(buffer);
    if (parser == NULL) { return; }
    
    // AUTO COMPILE SHADERS
    uint32_t compile_shaders = (uint32_t)json_object_get_int(json_object_object_get(parser, "auto_compile_shaders"));
    
    //MATERIALS
    json_object* json_materials = json_object_object_get(parser, "materials");
    uint32_t mat_count = (uint32_t)json_object_array_length(json_materials);
    ShMaterialHost* p_materials = calloc(mat_count, sizeof(ShMaterialHost));
    if (p_materials == NULL || mat_count == 0) { return; }
    for (uint32_t i = 0; i < mat_count; i++) {
        json_object* json_material = json_object_array_get_idx(json_materials, i);

        ShVkPipeline pipeline = { 0 };

        const uint32_t push_constant_size = (uint32_t)json_object_get_int(json_object_object_get(json_material, "push_constants_size"));
        const ShShaderStageFlags push_constant_stage = shStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "push_constants_stage")));
        shSetPushConstants(push_constant_stage, 0, push_constant_size, &pipeline);

        json_object* json_descriptors = json_object_object_get(json_material, "uniform_buffers");
        const uint8_t descriptor_buffer_count = (uint8_t)json_object_array_length(json_descriptors);
        if (json_descriptors != NULL) {
            for (uint8_t i = 0; i < descriptor_buffer_count; i++) {
                json_object* json_descriptor_buffer = json_object_array_get_idx(json_descriptors, i);
                const uint32_t set  = (uint32_t)json_object_get_int(json_object_object_get(json_descriptor_buffer, "set"));
                const uint32_t size = (uint32_t)json_object_get_int(json_object_object_get(json_descriptor_buffer, "size"));
                const uint8_t dynamic = (uint8_t)json_object_get_int(json_object_object_get(json_descriptor_buffer, "dynamic"));
                if (dynamic) {
                    shPipelineCreateDynamicDescriptorBuffer(p_core->device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, i, size, SH_MAX_UNIFORM_BUFFER_SIZE, &pipeline);
                }
                else {
                    shPipelineCreateDescriptorBuffer(p_core->device, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, i, size, &pipeline);
                }
                shPipelineAllocateDescriptorBufferMemory(p_core->device, p_core->physical_device, i, &pipeline);
                shPipelineBindDescriptorBufferMemory(p_core->device, i, &pipeline);
                shPipelineDescriptorSetLayout(p_core->device, 
                    set,
                    dynamic ? 1 : 0,
                    dynamic ? VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC : VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
                    shStringFlagToInt(json_object_get_string(json_object_object_get(json_descriptor_buffer, "stage"))), 
                    &pipeline
                );
                shPipelineCreateDescriptorPool(p_core->device, i, &pipeline);
                shPipelineAllocateDescriptorSet(p_core->device, i, &pipeline);
            }
        }

        uint32_t vertex_shader_size = 0;
        uint32_t fragment_shader_size = 0;
        char vertex_path[256];
        char fragment_path[256];
        shMakeAssetsPath(json_object_get_string(json_object_object_get(json_material, "vertex_shader")), vertex_path);
        shMakeAssetsPath(json_object_get_string(json_object_object_get(json_material, "fragment_shader")), fragment_path);
        const char* vertex_code = shReadBinary(
            vertex_path,
            &vertex_shader_size
        );
        const char* fragment_code = shReadBinary(
            fragment_path,
            &fragment_shader_size
        );
        shVkAssert(vertex_code != NULL, "invalid vertex shader");
        shVkAssert(fragment_code!= NULL, "invalid fragment shader");
        shPipelineCreateShaderModule(p_core->device, vertex_shader_size, vertex_code, &pipeline);
        shPipelineCreateShaderStage(p_core->device, VK_SHADER_STAGE_VERTEX_BIT, &pipeline);
        shPipelineCreateShaderModule(p_core->device, fragment_shader_size, fragment_code, &pipeline);
        shPipelineCreateShaderStage(p_core->device, VK_SHADER_STAGE_FRAGMENT_BIT, &pipeline);

        ShVkFixedStates fixed_states = { 0 };
        json_object* json_fixed_states = json_object_object_get(json_material, "fixed_states");
        json_object* json_vertex_inputs = json_object_object_get(json_fixed_states, "vertex_inputs");
        for (uint32_t i = 0; i < (uint32_t)json_object_array_length(json_vertex_inputs); i++) {
            json_object* json_vertex_input = json_object_array_get_idx(json_vertex_inputs, i);
            shSetVertexInputAttribute(
                (uint32_t)json_object_get_int(json_object_object_get(json_vertex_input, "location")),
                shStringFlagToInt(json_object_get_string(json_object_object_get(json_vertex_input, "format"))),
                (uint32_t)json_object_get_int(json_object_object_get(json_vertex_input, "offset")),
                (uint32_t)json_object_get_int(json_object_object_get(json_vertex_input, "size")),
                &fixed_states
            );
        }
        json_object* json_fixed_states_flags = json_object_object_get(json_fixed_states, "flags");
        ShFixedStateFlags fixed_state_flags = 0;
        if (json_fixed_states_flags != NULL) {
            for (uint32_t i = 0; i < json_object_array_length(json_fixed_states_flags); i++) {
                json_object* json_flag = json_object_array_get_idx(json_fixed_states_flags, i);
                fixed_state_flags |= shStringFlagToInt(json_object_get_string(json_flag));
            }
        }
	    shSetFixedStates(p_core->device, p_core->surface.width, p_core->surface.height, fixed_state_flags, &fixed_states);
        shSetupGraphicsPipeline(p_core->device, p_core->render_pass, fixed_states, &pipeline);
        p_materials[i].fixed_states = fixed_states;
        p_materials[i].pipeline = pipeline;
    }
    (pp_materials != NULL) && (*pp_materials = p_materials);
    (p_material_count != NULL) && (*p_material_count = mat_count);
    free(buffer);
}

#ifdef _MSC_VER
#pragma warning (disable: 6001)
#endif//_MSC_VER

void shMaterialsRelease(ShVkCore* p_core, uint32_t* p_mat_info_count, ShMaterialHost** pp_materials) {
	assert(p_mat_info_count != NULL && pp_materials != NULL);
	for (uint32_t i = 0; i < *p_mat_info_count; i++) {
        ShMaterialHost* p_material = &(*pp_materials)[i];
		for (uint32_t j = 0; j < p_material->pipeline.descriptor_count; j++) {
			shPipelineClearDescriptorBufferMemory(p_core->device, j, &p_material->pipeline);
		}
        shPipelineRelease(p_core->device, &(*pp_materials)[i].pipeline);

        for (uint32_t j = 0; j < p_material->entity_count; j++) {
            //if (p_material->material_clients[j].p_push_constant_parameters != NULL) {
            //    free(p_material->material_clients[j].p_push_constant_parameters);
            //}
            //if (p_material->material_clients[j].p_uniform_parameters != NULL) {
            //    free(p_material->material_clients[j].p_uniform_parameters);
            //}
        }
        
    }
	free(*pp_materials); *p_mat_info_count = 0;
}

void shReadUniformParameters(json_object* json_parameters, const uint32_t entity, const uint32_t descriptor_idx, ShMaterialHost* p_material) {
    assert(json_parameters != NULL && p_material != NULL);

    uint32_t descriptor_offset = shGetUniformOffset(p_material, descriptor_idx);

    for (uint32_t j = 0; j < (uint32_t)json_object_array_length(json_parameters); j += 2) {//FOR EACH UNIFORM PARAMETER VALUE
        const char* s_type = json_object_get_string(json_object_array_get_idx(json_parameters, j));

        if (strcmp(s_type, "float") == 0) {
            float value = (float)json_object_get_double(json_object_array_get_idx(json_parameters, j + 1));
            memcpy((void*)&((char*)p_material->material_clients[entity].p_uniform_parameters)[descriptor_offset], &value, 4);
        }
        if (strcmp(s_type, "int") == 0) {
            int value = (uint32_t)json_object_get_int(json_object_array_get_idx(json_parameters, j + 1));
            memcpy((void*)&((char*)p_material->material_clients[entity].p_uniform_parameters)[descriptor_offset], &value, 4);
        }
        if (strcmp(s_type, "transform") == 0) { //EXTENSION STRUCTURES
            p_material->extensions.transform_uniform_offset = descriptor_offset;
            descriptor_offset += 64;
        }
        descriptor_offset += 4;
    }
}

void shLoadScene(const char* path, ShMaterialHost** pp_materials, ShScene* p_scene) {
    assert(p_scene != NULL && pp_materials != NULL);

    char* buffer = (char*)shReadText(path, NULL);
    assert(buffer != NULL);

    json_object* parser = json_tokener_parse(buffer);
    assert(parser != NULL);

    //MESHES
    PlyFileData* ply_meshes = NULL;
    json_object* json_meshes = json_object_object_get(parser, "meshes");
    if (json_meshes) {
        uint32_t mesh_info_count = (uint32_t)json_object_array_length(json_meshes);
        ShMeshInfo* mesh_infos = calloc(mesh_info_count, sizeof(ShMeshInfo));

        uint32_t ply_mesh_count = 0;
        for (uint32_t i = 0; i < mesh_info_count; i++) {
            json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
            json_object_object_get(json_mesh, "path") != NULL && ply_mesh_count++;
        }
        ply_meshes = calloc(ply_mesh_count, sizeof(PlyFileData));
        for (uint32_t i = 0; i < ply_mesh_count; i++) {
            json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
            char mesh_path[256];
            json_object* json_path = json_object_object_get(json_mesh, "path");
            if (json_path != NULL) {
                shMakeAssetsPath(json_object_get_string(json_path), mesh_path);
                plyLoadFile(mesh_path, &ply_meshes[i], 0);
            }
        }
    }

    //ENTITIES
    json_object* json_entities = json_object_object_get(parser, "entities");
    for (uint32_t i = 0; i < json_object_array_length(json_entities); i++) {
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
                p_mesh_info->mesh_info.vertex_count = ply_meshes[data_index].vertex_count;
                p_mesh_info->mesh_info.vertex_stride = ply_meshes[data_index].vertex_stride;
                p_mesh_info->mesh_info.p_vertices = ply_meshes[data_index].p_vertices;
                p_mesh_info->mesh_info.index_count = ply_meshes[data_index].index_count;
                p_mesh_info->mesh_info.p_indices = ply_meshes[data_index].p_indices;
                json_object* json_flags = json_object_object_get(json_mesh, "flags");
                p_mesh_info->mesh_info.flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_MESH_SETUP_STATIC_MESH;
            }
        }
        if (json_camera != NULL) {
            ShCamera camera = {
                (float)json_object_get_double(json_object_object_get(json_camera, "fov")),
                (float)json_object_get_double(json_object_object_get(json_camera, "nc")),
                (float)json_object_get_double(json_object_object_get(json_camera, "fc")),
                (float)json_object_get_double(json_object_object_get(json_camera, "speed")),
                { 0 },
                { 0 },
                0
            };
            json_object* json_flags = json_object_object_get(json_camera, "flags");
            camera.flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_CAMERA_SETUP_FREE_FLIGHT;
            ShCamera* p_camera = shAddShCamera(p_scene, entity);
            *p_camera = camera;
        }
        if (json_material != NULL) {
            const uint32_t idx = json_object_get_int(json_object_object_get(json_material, "index"));
            ShMaterialHost* p_material = &(*pp_materials)[idx];
            for (uint32_t j = 0; j < p_material->pipeline.descriptor_count; j++) {
                p_material->pipeline.write_descriptor_sets[j].pBufferInfo = &p_material->pipeline.descriptor_buffer_infos[j];
            }
            p_material->entities[p_material->entity_count] = entity;
        
            json_object* json_push_constant_parameters = json_object_object_get(json_material, "push_constant_parameters");
            json_object* json_descriptor_parameters = json_object_object_get(json_material, "uniform_parameters");

            //if (json_push_constant_parameters != NULL) {
            //    for (uint32_t push_constant_idx = 0; push_constant_idx < json_object_array_length(json_push_constant_parameters); push_constant_idx++) {//for each push constant (single)
            //        shReadMaterialParameters(json_object_array_get_idx(json_push_constant_parameters, push_constant_idx), 0, &p_material->material_clients[entity]);
            //    }
            //}
            
            if (json_descriptor_parameters != NULL) {
                for (uint32_t descriptor_idx = 0; descriptor_idx < json_object_array_length(json_descriptor_parameters); descriptor_idx++) {//for each descriptor
                    shReadUniformParameters(json_object_array_get_idx(json_descriptor_parameters, descriptor_idx), entity, descriptor_idx, p_material);
                    p_material->material_clients[entity].parameters = 1;
                }
            }

            //float values[256];
            //uint32_t sz = shGetUniformTotalSize(p_material);
            //memcpy(values, p_material->material_clients[entity].p_uniform_parameters, sz);

            p_material->entity_count++;
        }
        if (json_identity != NULL) {
            ShIdentity* p_identity  = shAddShIdentity(p_scene, entity);
            json_object* json_name   = json_object_object_get(json_identity, "name");
            json_object* json_tag    = json_object_object_get(json_identity, "tag");
            json_object* json_subtag = json_object_object_get(json_identity, "subtag");
            (json_name   != NULL) && (p_identity->name = json_object_get_string(json_name));
            (json_tag    != NULL) && (p_identity->tag = json_object_get_string(json_tag));
            (json_subtag != NULL) && (p_identity->subtag = json_object_get_string(json_subtag));
        }
        if (json_molecule != NULL) {
            
        }
#if 0
        if (json_physics_client != NULL) {
            ShPhysicsClient* client = shAddShPhysicsClient(p_scene, entity);
            for (uint32_t j = 0; j < json_object_array_length(json_physics_client); j++) {
                *client |= shStringFlagToInt(json_object_get_string(json_object_array_get_idx(json_physics_client, j)));
            }
        }
        if (json_rigidbody != NULL) {
            json_object* json_mass = json_object_object_get(json_rigidbody, "mass");
            json_object* json_shape = json_object_object_get(json_rigidbody, "shape");
            
            shreal mass = DEC(0.0);
            ShCollisionShapeType shape_type = 0;

            (json_mass   != NULL) && (mass = (shreal)json_object_get_double(json_mass));
            (json_shape != NULL) && (shape_type = shStringFlagToInt(json_object_get_string(json_shape)));
            
            //ShRigidBody* p_rb = shAddShRigidBody(p_scene, entity);
            //shDynamicsRigidBodyInit(mass, shape_type, p_rb);

            if (shape_type == SH_COLLISION_SHAPE_SPHERE) { 
                shreal radius = DEC(0.0);
                json_object* json_radius = json_object_object_get(json_rigidbody, "radius");
                (json_radius != NULL) && (radius = (shreal)json_object_get_double(json_radius));
                //shDynamicsSetCollisionSphereRadius(radius, p_rb);
            }
        }
#endif
    }
    
    if (ply_meshes != NULL) { free(ply_meshes); }
    free(buffer);
}

void shLoadPhysicsWorld(const char* path, ShPhysicsHost* p_host) {
    assert(p_host != NULL);
    char* buffer = (char*)shReadText(path, NULL);
    if (buffer == NULL) { return; }

    json_object* parser = json_tokener_parse(buffer);
    if (parser == NULL) { return; }

    json_object* json_physics_host = json_object_object_get(parser, "physics_host");
    if (json_physics_host != NULL) {
        json_object* json_electrostatic = json_object_object_get(json_physics_host, "electrostatic_charges");
        for (uint32_t i = 0; i < json_object_array_length(json_electrostatic); i++) {
            json_object* json_charge = json_object_array_get_idx(json_electrostatic, i);
            json_object* json_position = json_object_object_get(json_charge, "position");
            if (json_position != NULL) {
                for (uint32_t j = 0; j < 3; j++) {
                    json_object* json_pos = json_object_array_get_idx(json_position, j);
                    p_host->electrostatic_world.charges[i].position[j] = json_pos != NULL ? (float)json_object_get_double(json_pos) : 0.0f;
                }
            }
            json_object* json_intensity = json_object_object_get(json_charge, "intensity");
            p_host->electrostatic_world.charges[i].intensity[0] = json_intensity != NULL ? (float)json_object_get_double(json_intensity) : 0.0f;
            //for (uint32_t j = 0; j < 4; j++) {
            //    json_object* json_intens = json_object_array_get_idx(json_intensity, j);
            //    p_host->electrostaticWorld.charges[i].intensity[j] = json_intens != NULL ? (float)json_object_get_double(json_intens) : 0.0f;
            //}
            //
        }
    }
    
#if 0
    json_object* json_dynamics  = json_object_object_get(parser, "dynamics_world");
    json_object* json_speed     = json_object_object_get(parser, "speed");
    if (json_dynamics != NULL) {
        ShDynamicsWorld dynamics = { 0 };
        for (uint32_t i = 0; i < (uint32_t)json_object_array_length(json_dynamics); i++) {
            json_object* json_flag = json_object_array_get_idx(json_dynamics, i);
            dynamics.flags |= shStringFlagToInt(json_object_get_string(json_flag));
        }
        (json_speed != NULL) && (dynamics.speed = (shreal)json_object_get_double(json_speed));
        uint32_t* scene_indices = (uint32_t*)calloc(p_scene->entity_count, sizeof(uint32_t));
        if (scene_indices != NULL) {
            for (uint32_t entity = 0; entity < p_scene->entity_count; entity++) {
                if (shHasShRigidBody(p_scene, entity)) {
                    scene_indices[dynamics.rbody_count] = entity;
                    dynamics.rbody_count++;
                }
            }
            dynamics.pp_rbodies = calloc(dynamics.rbody_count, sizeof(ShRigidBody));
            if (dynamics.pp_rbodies != NULL) {
                for (uint32_t i = 0; i < dynamics.rbody_count; i++) {
                    dynamics.pp_rbodies[i] = shGetShRigidBody(p_scene, scene_indices[i]);
                }
            }
            free(scene_indices);
        }
        *p_dynamics = dynamics;
    }
#endif
    free(buffer);
}

uint8_t shListenFd(ShFd* descriptor_handle) {
    shGetFileStats(descriptor_handle->path, &descriptor_handle->stats1);
    if (memcmp(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(ShFileStats)) != 0) {
        memcpy(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(ShFileStats));
#ifndef NDEBUG
        printf("Saved descriptor at %s\n", descriptor_handle->path);
#endif
        return 1;
    }
    return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
