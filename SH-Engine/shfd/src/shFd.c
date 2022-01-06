#include "shfd/shFd.h"
#include "shfd/shFile.h"

#include "ecs/shCamera.h"
#include "ecs/shTransform.h"
#include "ecs/shIdentity.h"
#include "ecs/shPhysics.h"
#include "ecs/shMaterial.h"
#include "ecs/shMesh.h"

#include <sys/types.h>
#include <sys/stat.h>

#include <time.h>
#include <stdio.h>
#include <memory.h>
#include <string.h>

#ifndef NDEBUG
#include <stdio.h>
#endif

#include <plyimporter/PlyImporter.h>

#ifdef _MSC_VER
#pragma warning (disable: 26451 6386)
#endif // _MSC_VER
#include <json.h>

void shGetFileStats(const char* path, ShFileStats* stats) {
    stat(path, stats);
}

void shInitDescriptor(ShFd* descriptor_handle) {
    shGetFileStats(descriptor_handle->path, &descriptor_handle->stats0);
}

uint32_t shStringFlagToInt(const char* s_flag) {
    if (strcmp(s_flag, "SHADER_STAGE_VERTEX") == 0) {
        return SH_SHADER_STAGE_VERTEX;
    }
    if (strcmp(s_flag, "SHADER_STAGE_FRAGMENT") == 0) {
        return SH_SHADER_STAGE_FRAGMENT;
    }
    if (strcmp(s_flag, "SHADER_STAGE_GEOMETRY") == 0) {
        return SH_SHADER_STAGE_GEOMETRY;
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
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_POSITIONS") == 0) {
        return SH_FIXED_STATES_VERTEX_POSITIONS;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_NORMALS") == 0) {
        return SH_FIXED_STATES_VERTEX_NORMALS;
    }
    if (strcmp(s_flag, "FIXED_STATES_VERTEX_TCOORDS") == 0) {
        return SH_FIXED_STATES_VERTEX_TCOORDS;
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
    if (strcmp(s_flag, "MESH_SETUP_RUNTIME_MESH") == 0) {
        return SH_MESH_SETUP_RUNTIME_MESH;
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

void shLoadMaterials(const char* path, uint32_t* p_material_count, ShMaterial** pp_materials) {
    assert(p_material_count != NULL && pp_materials != NULL);

    char* buffer = (char*)shReadText(path, NULL);
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
            const char* bin = json_object_get_string(json_object_array_get_idx(json_shader_sources, (size_t)i + 1));
            ////shCompileGLSLShader(shader_source, bin);
        }
    }

    //MATERIALS
    json_object* json_materials = json_object_object_get(parser, "materials");
    uint32_t mat_count = (uint32_t)json_object_array_length(json_materials);
    ShMaterial* p_materials = calloc(mat_count, sizeof(ShMaterial));
    if (p_materials == NULL || mat_count == 0) { return; }
    for (uint32_t i = 0; i < mat_count; i++) {
        json_object* json_material = json_object_array_get_idx(json_materials, i);
        ShMaterial material = {
            json_object_get_string(json_object_object_get(json_material, "vertex_shader")),                         // vertex_shader_path;
            json_object_get_string(json_object_object_get(json_material, "fragment_shader")),                       // fragment_shader_path;	
            0,
            NULL,
            (uint32_t)json_object_get_int(json_object_object_get(json_material, "push_constants_size")),                // pConstSize; 
            shStringFlagToInt(json_object_get_string(json_object_object_get(json_material, "push_constants_stage"))),  // pConstStage;
            0                                                                                                           // fixed_states_flags;
        };
        json_object* json_fixed_states_flags = json_object_object_get(json_material, "fixed_states");
        if (json_fixed_states_flags != NULL) {
            for (uint32_t i = 0; i < json_object_array_length(json_fixed_states_flags); i++) {
                json_object* json_flag = json_object_array_get_idx(json_fixed_states_flags, i);
                ShFixedStateFlags flag = shStringFlagToInt(json_object_get_string(json_flag));
                material.material_info.fixed_states_flags |= flag;
            }
        }
        json_object* json_uniforms = json_object_object_get(json_material, "uniform_buffers");
        material.material_info.uniform_buffer_count = (uint32_t)json_object_array_length(json_uniforms);
        if (json_uniforms != NULL) {
            material.material_info.p_uniform_buffers = calloc(material.material_info.uniform_buffer_count, sizeof(ShUniformBufferInfo));
            for (uint32_t i = 0; i < material.material_info.uniform_buffer_count; i++) {
                json_object* json_uniform_buffer = json_object_array_get_idx(json_uniforms, i);
                material.material_info.p_uniform_buffers[i].uniform_size = (uint32_t)json_object_get_int(json_object_object_get(json_uniform_buffer, "size"));
                material.material_info.p_uniform_buffers[i].uniform_stage = shStringFlagToInt(json_object_get_string(json_object_object_get(json_uniform_buffer, "stage")));
            }
        }
        p_materials[i] = material;
    }
    (pp_materials != NULL) && (*pp_materials = p_materials);
    (p_material_count != NULL) && (*p_material_count = mat_count);
    free(buffer);
}

void shLoadScene(const char* path, const ShMaterial* p_materials, ShScene* p_scene) {
    assert(p_scene != NULL && p_materials != NULL);

    char* buffer = (char*)shReadText(path, NULL);
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
        json_object* json_mesh      = json_object_object_get(json_entity, "mesh");
        json_object* json_camera    = json_object_object_get(json_entity, "camera");
        json_object* json_material  = json_object_object_get(json_entity, "material");
        json_object* json_identity  = json_object_object_get(json_entity, "identity");
        json_object* json_physics_client = json_object_object_get(json_entity, "physics_client");

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
            json_object* json_data_index = json_object_object_get(json_mesh, "data_index");
            if (json_data_index != NULL) {
                uint32_t data_index = json_object_get_int(json_data_index);
                p_mesh_info->mesh_info.vertex_count = ply_meshes[data_index].vertexCount * ply_meshes[data_index].vertexStride;
                p_mesh_info->mesh_info.p_vertices = ply_meshes[data_index].pVertices;
                p_mesh_info->mesh_info.index_count = ply_meshes[data_index].indexCount;
                p_mesh_info->mesh_info.p_indices = ply_meshes[data_index].pIndices;
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
            ShMaterial* p_material = shAddShMaterial(p_scene, entity);
            uint32_t material_index = json_object_get_int(json_material);
            *p_material = p_materials[material_index];
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
        if (json_physics_client != NULL) {
            ShPhysicsClient* client = shAddShPhysicsClient(p_scene, entity);
            for (uint32_t j = 0; j < json_object_array_length(json_physics_client); j++) {
                *client |= shStringFlagToInt(json_object_get_string(json_object_array_get_idx(json_physics_client, j)));
            }
        }
#if 0
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
    
    free(ply_meshes);
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