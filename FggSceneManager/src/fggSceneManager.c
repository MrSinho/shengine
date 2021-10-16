#include "fggSceneManager.h"
#include "fggUtilities.h"
#include "fggComponents.h"

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
    if (strcmp(s_flag, "FIXED_STATES_FIXED_STATES_VERTEX_NORMALS") == 0) {
        return FGG_FIXED_STATES_VERTEX_NORMALS_BIT;
    }
    if (strcmp(s_flag, "FIXED_STATES_FIXED_STATES_VERTEX_TCOORDS") == 0) {
        return FGG_FIXED_STATES_VERTEX_TCOORDS_BIT;
    }
    return 0;
}

void fggLoadScene(const char* path) {
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
    
    //MATERIALS
    json_object* json_materials = json_object_object_get(parser, "materials");
    uint32_t material_info_count = json_object_array_length(json_materials);
    FggMaterialInfo* p_material_infos = calloc(material_info_count, sizeof(FggMaterialInfo));
    if (p_material_infos == NULL) { return; }
    for (uint32_t i = 0; i < material_info_count; i++) {
        json_object* json_material = json_object_array_get_idx(json_materials, i);
        json_object* json_vertex_shader_path =      json_object_object_get(json_material, "vertex_shader_path");
        json_object* json_fragment_shader_path =    json_object_object_get(json_material, "fragment_shader_path");
        json_object* json_uniform_size =            json_object_object_get(json_material, "uniform_size");
        json_object* json_uniform_stage =           json_object_object_get(json_material, "uniform_stage");
        json_object* json_push_constants_size =     json_object_object_get(json_material, "push_constants_size");
        json_object* json_push_constants_stage =    json_object_object_get(json_material, "push_constants_stage");
        json_object * json_fixed_state_flags =      json_object_object_get(json_material, "fixed_state_flags");
        FggMaterialInfo material_info = {
            json_object_get_string(json_vertex_shader_path),                                // vertex_shader_path;
            json_object_get_string(json_fragment_shader_path),                              // fragment_shader_path;	
            (uint32_t)json_object_get_int(json_uniform_size),                               // uniformSize;
            fggStringFlagToInt(json_object_get_string(json_uniform_stage)),                 // uniformStage;
            (uint32_t)json_object_get_int(json_push_constants_size),                        // pConstSize; 
            fggStringFlagToInt(json_object_get_string(json_push_constants_stage)),          // pConstStage;
            0                                                                               // fixed_states_flags;
        };
        json_object* json_fixed_states_flags = json_object_object_get(json_material, "fixed_states");
        for (uint32_t i = 0; i < json_object_array_length(json_fixed_states_flags); i++) {
            json_object* json_flag = json_object_array_get_idx(json_fixed_states_flags, i);
            FggFixedStateFlags flag = fggStringFlagToInt(json_object_get_string(json_flag));
            material_info.fixed_states_flags |= flag;
        }
        p_material_infos[i] = material_info;
    }
    
    //ENTITIES
    json_object* json_entities = json_object_object_get(parser, "entities");
    for (uint32_t i = 0; i < json_object_array_length(json_entities); i++) {
        //uint32_t entity = fggCreateEntity();
    }


    free(p_material_infos);
    free(buffer);
    fclose(stream);
}

void fggListenSceneDescriptor(FggSceneDescriptorHandle* descriptor_handle, FggScene scene) {
    fggGetFileStats(descriptor_handle->path, &descriptor_handle->stats1);
    if (memcmp(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(FggFileStats)) != 0) {
        memcpy(&descriptor_handle->stats0, &descriptor_handle->stats1, sizeof(FggFileStats));
#ifndef NDEBUG
        printf("Saved scene descriptor at %s\n", descriptor_handle->path);
#endif
        fggLoadScene(descriptor_handle->path);
    }
}