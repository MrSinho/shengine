#include "fggSceneManager.h"
#include "fggUtilities.h"
#include "fggCamera.h"
#include "fggTransform.h"
#include "fggMaterialInfo.h"

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


void fggGetFileStats(const char* path, FggFileStats* stats) {
    stat(path, stats);
}

void fggInitSceneDescriptor(FggSceneDescriptorHandle* descriptor_handle) {
    fggGetFileStats(descriptor_handle->path, &descriptor_handle->stats0);
}

uint32_t fggStringFlagToInt(const char* s_flag) {
    if (strcmp(s_flag, "SHADER_STAGE_VERTEX") == 0) {
        return FGG_SHADER_STAGE_VERTEX_BIT;
    }
    if (strcmp(s_flag, "SHADER_STAGE_FRAGMENT") == 0) {
        return FGG_SHADER_STAGE_FRAGMENT_BIT;
    }
    if (strcmp(s_flag, "SHADER_STAGE_GEOMETRY") == 0) {
        return FGG_SHADER_STAGE_GEOMETRY_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_WIREFRAME") == 0) {
        return FGG_FIXED_STATES_POLYGON_MODE_WIREFRAME_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_FACE") == 0) {
        return FGG_FIXED_STATES_POLYGON_MODE_FACE_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_POINTS") == 0) {
        return FGG_FIXED_STATES_POLYGON_MODE_POINTS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_TRIANGLE_LIST") == 0) {
        return FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_LINE_LIST") == 0) {
        return FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_LINE_LIST;
    }
    if (strcmp(s_flag, "FIXED_STATES_POLYGON_MODE_POINT_LIST") == 0) {
        return FGG_FIXED_STATES_PRIMITIVE_TOPOLOGY_POINT_LIST;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_POSITIONS") == 0) {
        return FGG_FIXED_STATES_VERTEX_POSITIONS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_NORMALS") == 0) {
        return FGG_FIXED_STATES_VERTEX_NORMALS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_TCOORDS") == 0) {
        return FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_FREE_FLIGHT") == 0) {
        return FGG_CAMERA_SETUP_FREE_FLIGHT_BIT;
    }
    if (strcmp(s_flag, "CAMERA_SETUP_STATIC") == 0) {
        return FGG_CAMERA_SETUP_STATIC_BIT;
    }
    if (strcmp(s_flag, "MESH_SETUP_STATIC_MESH") == 0) {
        return FGG_MESH_SETUP_STATIC_MESH;
    }
    if (strcmp(s_flag, "MESH_SETUP_DYNAMIC_MESH") == 0) {
        return FGG_MESH_SETUP_DYNAMIC_MESH;
    }
    if (strcmp(s_flag, "MESH_SETUP_RUNTIME_MESH") == 0) {
        return FGG_MESH_SETUP_RUNTIME_MESH;
    }
    return 0;
}

void fggLoadScene(const char* path, FggScene* p_scene) {
    FILE* stream = fopen(path, "r");
    if (stream == NULL) {
#ifndef NDEBUG
        printf("FggError: cannot open scene descriptor at %s\n", path);
#endif
        return;
    }

    fseek(stream, 0, SEEK_END);
    uint32_t descriptor_size = ftell(stream);
    fseek(stream, 0, SEEK_SET);

    char* buffer = calloc(descriptor_size, 1);
    if (buffer == NULL) { return; }
    fread(buffer, descriptor_size, 1, stream);

    json_object* parser = json_tokener_parse(buffer);
    if (parser == NULL) { return; }

    //MATERIALS
    json_object* json_materials = json_object_object_get(parser, "materials");
    uint32_t material_info_count = (uint32_t)json_object_array_length(json_materials);
    FggMaterialInfo* p_material_infos = calloc(material_info_count, sizeof(FggMaterialInfo));
    if (p_material_infos == NULL) { return; }
    for (uint32_t i = 0; i < material_info_count; i++) {
        json_object* json_material = json_object_array_get_idx(json_materials, i);
        FggMaterialInfo material_info = {
            json_object_get_string(json_object_object_get(json_material, "vertex_shader_path")),                        // vertex_shader_path;
            json_object_get_string(json_object_object_get(json_material, "fragment_shader_path")),                      // fragment_shader_path;	
            (uint32_t)json_object_get_int(json_object_object_get(json_material, "uniform_size")),                       // uniformSize;
            fggStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "uniform_stage"))),         // uniformStage;
            (uint32_t)json_object_get_int(json_object_object_get(json_material, "push_constants_size")),                // pConstSize; 
            fggStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "push_constants_stage"))),  // pConstStage;
            0                                                                                                           // fixed_states_flags;
        };
        json_object* json_fixed_states_flags = json_object_object_get(json_material, "fixed_states");
        if (json_fixed_states_flags != NULL) {
            for (uint32_t i = 0; i < json_object_array_length(json_fixed_states_flags); i++) {
                json_object* json_flag = json_object_array_get_idx(json_fixed_states_flags, i);
                FggFixedStateFlags flag = fggStringFlagToInt(json_object_get_string(json_flag));
                material_info.fixed_states_flags |= flag;
            }
        }
        p_material_infos[i] = material_info;
    }

    //ENTITIES
    //fggClearScene(p_scene);
    json_object* json_entities = json_object_object_get(parser, "entities");
    for (uint32_t i = 0; i < json_object_array_length(json_entities); i++) {
        uint32_t entity = fggCreateEntity(p_scene);
        json_object* json_entity = json_object_array_get_idx(json_entities, i);

        json_object* json_transform = json_object_object_get(json_entity, "transform");
        json_object* json_mesh = json_object_object_get(json_entity, "mesh");
        json_object* json_camera = json_object_object_get(json_entity, "camera");
        json_object* json_material = json_object_object_get(json_entity, "material");

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
                    json_object* json_mdl = json_object_array_get_idx(json_mdl, j);
                    model[j] = json_mdl != NULL ? (float)json_object_get_double(json_mdl) : 0.0f;
                }
            }
            FggTransform* p_transform = fggAddFggTransform(p_scene, entity);
            memcpy(p_transform->model, model, 64);
            memcpy(p_transform->position, position, 12);
            memcpy(p_transform->scale, scale, 12);
            memcpy(p_transform->rotation, rotation, 12);
        }
        if (json_mesh != NULL) {
            FggMeshInfo* p_mesh_info = fggAddFggMeshInfo(p_scene, entity);
            if (json_object_object_get(json_mesh, "path") != NULL) {
                PlyFileData ply = { 0 };
                plyLoadFile(json_object_get_string(json_object_object_get(json_mesh, "path")), &ply, 0);
                p_mesh_info->vertex_count = ply.vertexCount * ply.vertexStride;
                p_mesh_info->p_vertices = ply.pVertices;
                p_mesh_info->index_count = ply.indexCount;
                p_mesh_info->p_indices = ply.pIndices;
                plyFree(&ply);
            }
            json_object* json_flags = json_object_object_get(json_mesh, "flags");
            p_mesh_info->flags = json_flags != NULL ? fggStringFlagToInt(json_object_get_string(json_flags)) : FGG_MESH_SETUP_STATIC_MESH;
        }
        if (json_camera != NULL) {
            FggCamera camera = {
                (float)json_object_get_double(json_object_object_get(json_camera, "fov")),
                (float)json_object_get_double(json_object_object_get(json_camera, "nc")),
                (float)json_object_get_double(json_object_object_get(json_camera, "fc")),
                { 0 },
                { 0 },
                0
            };
            json_object* json_flags = json_object_object_get(json_camera, "flags");
            camera.flags = json_flags != NULL ? fggStringFlagToInt(json_object_get_string(json_flags)) : FGG_CAMERA_SETUP_FREE_FLIGHT_BIT;
            FggCamera* p_camera = fggAddFggCamera(p_scene, entity);
            *p_camera = camera;
        }
        if (json_material != NULL) {
            FggMaterialInfo* p_material_info = fggAddFggMaterialInfo(p_scene, entity);
            uint32_t material_info_index = json_object_get_int(json_material);
            *p_material_info = p_material_infos[material_info_index];
        }
    }




    free(p_material_infos);
    free(buffer);
    fclose(stream);
}

int fggListenSceneDescriptor(FggSceneDescriptorHandle* descriptor_handle, FggScene* p_scene) {
    fggGetFileStats(descriptor_handle->path, &descriptor_handle->stats1);
    if (memcmp(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(FggFileStats)) != 0) {
        memcpy(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(FggFileStats));
#ifndef NDEBUG
        printf("Saved scene descriptor at %s\n", descriptor_handle->path);
#endif
        return 1;
    }
    return 0;
}