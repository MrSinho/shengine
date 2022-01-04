#ifndef SH_SCENE_HANDLE_H
#define SH_SCENE_HANDLE_H

#include "ecs/shEcsImplementation.h"

#include <stdint.h>

typedef struct ShEngine ShEngine;

extern void shSceneInit(ShEngine* p_engine, const uint32_t scene_idx);

extern void shSceneUpdate(ShEngine* p_engine, const uint32_t scene_idx);

extern void shRenderMesh(const ShVkCore engine, const ShVkPipelineData pipe_data, const uint32_t push_const_size, void* p_push_const, const uint32_t uniforms_size, void* p_uniforms, ShMeshInfo* p_mesh_info, ShMesh* mesh);

extern void shSceneRelease(ShEngine* p_engine, const uint32_t scene_idx);

#endif