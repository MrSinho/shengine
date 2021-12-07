#include "shDescriptorHandle.h"
#include "shUtilities.h"
#include "shCamera.h"
#include "shTransform.h"
#include "shMaterialInfo.h"
#include "shIdentity.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

#include <json.h>

#include <PlyImporter.h>

#ifdef _MSC_VER
#pragma warning ( disable : 4996)
#endif // _MSC_VER

void shGetFileStats(const char* path, ShFileStats* stats) {
    stat(path, stats);
}

void shInitDescriptor(ShDescriptorHandle* descriptor_handle) {
    shGetFileStats(descriptor_handle->path, &descriptor_handle->stats0);
}

uint32_t shStringFlagToInt(const char* s_flag) {
    if (strcmp(s_flag, "SHADER_STAGE_VERTEX") == 0) {
        return SH_SHADER_STAGE_VERTEX_BIT;
    }
    if (strcmp(s_flag, "SHADER_STAGE_FRAGMENT") == 0) {
        return SH_SHADER_STAGE_FRAGMENT_BIT;
    }
    if (strcmp(s_flag, "SHADER_STAGE_GEOMETRY") == 0) {
        return SH_SHADER_STAGE_GEOMETRY_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_WIREFRAME") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_FACE") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_FACE_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_POINTS") == 0) {
        return SH_FIXED_STATES_POLYGON_MODE_POINTS_BIT;
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
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_POSITIONS") == 0) {
        return SH_FIXED_STATES_VERTEX_POSITIONS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_NORMALS") == 0) {
        return SH_FIXED_STATES_VERTEX_NORMALS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_TCOORDS") == 0) {
        return SH_FIXED_STATES_VERTEX_TCOORDS_BIT;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_FREE_FLIGHT") == 0) {
        return SH_CAMERA_SETUP_FREE_FLIGHT_BIT;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_STATIC") == 0) {
        return SH_CAMERA_SETUP_STATIC_BIT;
    }
    if (strcmp(s_flag, "MESH_SETUP_STATIC_MESH") == 0) {
        return SH_MESH_SETUP_STATIC_MESH;
    }
    if (strcmp(s_flag, "MESH_SETUP_DYNAMIC_MESH") == 0) {
        return SH_MESH_SETUP_DYNAMIC_MESH;
    }
    if (strcmp(s_flag, "MESH_SETUP_RUNTIME_MESH") == 0) {
        return SH_MESH_SETUP_RUNTIME_MESH;
    }
    return 0;
}

void shLoadMaterialInfos(const char* path, uint32_t* p_mat_info_count, ShMaterialInfo** pp_mat_infos) {
    char* buffer = (char*)shReadCode(path, NULL, "r");
    if (buffer == NULL) { return; }

    json_object* parser = json_tokener_parse(buffer);
    if (parser == NULL) { return; }
    
    // AUTO COMPILE SHADERS
    uint32_t compile_shaders = (uint32_t)json_object_get_int(json_object_object_get(parser, "auto_compile_shaders"));
    
    // SHADER SOURCES
    json_object* json_shader_sources = json_object_object_get(parser, "shader_sources");
    uint32_t shader_source_count = (uint32_t)json_object_array_length(json_shader_sources);
    if (compile_shaders > 0) {
        for (uint32_t i = 0; i < shader_source_count; i += 2) {
            const char* shader_source = json_object_get_string(json_object_array_get_idx(json_shader_sources, i));
            const char* bin = json_object_get_string(json_object_array_get_idx(json_shader_sources, i + 1));
            shCompileGLSLShader(shader_source, bin);
        }
    }

    //MATERIALS
    json_object* json_materials = json_object_object_get(parser, "materials");
    uint32_t mat_info_count = (uint32_t)json_object_array_length(json_materials);
    ShMaterialInfo* p_mat_infos = calloc(mat_info_count, sizeof(ShMaterialInfo));
    if (p_mat_infos == NULL) { return; }
    for (uint32_t i = 0; i < mat_info_count; i++) {
        json_object* json_material = json_object_array_get_idx(json_materials, i);
        ShMaterialInfo material_info = {
            json_object_get_string(json_object_object_get(json_material, "vertex_shader")),                         // vertex_shader_path;
            json_object_get_string(json_object_object_get(json_material, "fragment_shader")),                       // fragment_shader_path;	
            (uint32_t)json_object_get_int(json_object_object_get(json_material, "uniform_size")),                       // uniformSize;
            shStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "uniform_stage"))),         // uniformStage;
            (uint32_t)json_object_get_int(json_object_object_get(json_material, "push_constants_size")),                // pConstSize; 
            shStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "push_constants_stage"))),  // pConstStage;
            0                                                                                                           // fixed_states_flags;
        };
        json_object* json_fixed_states_flags = json_object_object_get(json_material, "fixed_states");
        if (json_fixed_states_flags != NULL) {
            for (uint32_t i = 0; i < json_object_array_length(json_fixed_states_flags); i++) {
                json_object* json_flag = json_object_array_get_idx(json_fixed_states_flags, i);
                ShFixedStateFlags flag = shStringFlagToInt(json_object_get_string(json_flag));
                material_info.fixed_states_flags |= flag;
            }
        }
        p_mat_infos[i] = material_info;
    }
    (p_mat_infos != NULL) && (*pp_mat_infos = p_mat_infos);
    (p_mat_info_count != NULL) && (*p_mat_info_count = mat_info_count);
    free(buffer);
}

void shLoadScene(const char* path, const ShMaterialInfo* p_mat_infos, ShScene* p_scene) {
    char* buffer = (char*)shReadCode(path, NULL, "r");
    if (buffer == NULL) { return; }

    json_object* parser = json_tokener_parse(buffer);
    if (parser == NULL) { return; }

    

    //MESHES
    json_object* json_meshes = json_object_object_get(parser, "meshes");
    uint32_t mesh_info_count = (uint32_t)json_object_array_length(json_meshes);
    ShMeshInfo* mesh_infos = calloc(mesh_info_count, sizeof(ShMeshInfo));
    
    uint32_t ply_mesh_count = 0; 
    for (uint32_t i = 0; i < mesh_info_count; i++) {
        json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
        json_object_get_string(json_object_object_get(json_mesh, "path")) != NULL && ply_mesh_count++;
    }
    PlyFileData* ply_meshes = calloc(ply_mesh_count, sizeof(PlyFileData));
    for (uint32_t i = 0; i < ply_mesh_count; i++) {
        json_object* json_mesh = json_object_array_get_idx(json_meshes, i);
        const char* path = json_object_get_string(json_object_object_get(json_mesh, "path"));
        if (path != NULL) { plyLoadFile(path, &ply_meshes[i], 0); }
    }

    //ENTITIES
    json_object* json_entities = json_object_object_get(parser, "entities");
    for (uint32_t i = 0; i < json_object_array_length(json_entities); i++) {
        uint32_t entity = shCreateEntity(p_scene);
        json_object* json_entity = json_object_array_get_idx(json_entities, i);

        json_object* json_transform = json_object_object_get(json_entity, "transform");
        json_object* json_mesh = json_object_object_get(json_entity, "mesh");
        json_object* json_camera = json_object_object_get(json_entity, "camera");
        json_object* json_material = json_object_object_get(json_entity, "material");
        json_object* json_identity = json_object_object_get(json_entity, "identity");

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
            ShMeshInfo* p_mesh_info = shAddShMeshInfo(p_scene, entity);
            json_object* json_data_index = json_object_object_get(json_mesh, "data_index");
            if (json_data_index != NULL) {
                uint32_t data_index = json_object_get_int(json_data_index);
                p_mesh_info->vertex_count = ply_meshes[data_index].vertexCount * ply_meshes[data_index].vertexStride;
                p_mesh_info->p_vertices = ply_meshes[data_index].pVertices;
                p_mesh_info->index_count = ply_meshes[data_index].indexCount;
                p_mesh_info->p_indices = ply_meshes[data_index].pIndices;
                json_object* json_flags = json_object_object_get(json_mesh, "flags");
                p_mesh_info->flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_MESH_SETUP_STATIC_MESH;
            }
        }
        if (json_camera != NULL) {
            ShCamera camera = {
                (float)json_object_get_double(json_object_object_get(json_camera, "fov")),
                (float)json_object_get_double(json_object_object_get(json_camera, "nc")),
                (float)json_object_get_double(json_object_object_get(json_camera, "fc")),
                { 0 },
                { 0 },
                0
            };
            json_object* json_flags = json_object_object_get(json_camera, "flags");
            camera.flags = json_flags != NULL ? shStringFlagToInt(json_object_get_string(json_flags)) : SH_CAMERA_SETUP_FREE_FLIGHT_BIT;
            ShCamera* p_camera = shAddShCamera(p_scene, entity);
            *p_camera = camera;
        }
        if (json_material != NULL) {
            ShMaterialInfo* p_material_info = shAddShMaterialInfo(p_scene, entity);
            uint32_t material_info_index = json_object_get_int(json_material);
            *p_material_info = p_mat_infos[material_info_index];
        }
        if (json_identity != NULL) {
            ShIdentity* p_identity  = shAddShIdentity(p_scene, entity);
            json_object* json_name   = json_object_object_get(json_identity, "name");
            json_object* json_tag    = json_object_object_get(json_identity, "tag");
            json_object* json_subtag = json_object_object_get(json_identity, "subtag");
            json_name   != NULL && (p_identity->name = json_object_get_string(json_name));
            json_tag    != NULL && (p_identity->tag = json_object_get_string(json_tag));
            json_subtag != NULL && (p_identity->subtag = json_object_get_string(json_subtag));
        }
    }


    free(ply_meshes);
    free(buffer);
}

int shListenDescriptor(ShDescriptorHandle* descriptor_handle) {
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