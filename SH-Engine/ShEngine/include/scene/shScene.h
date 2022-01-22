#ifndef SH_SCENE_HANDLE_H
#define SH_SCENE_HANDLE_H

#include "ecs/shEcsImplementation.h"

#include <stdint.h>

typedef struct ShEngine ShEngine;

typedef struct ShMeshInfo ShMeshInfo;

typedef struct ShTransform ShTransform;

typedef struct ShCamera ShCamera;

extern void shSceneInit(ShEngine* p_engine, const uint32_t scene_idx);

extern void shUpdateShTransform(ShTransform* p_transform);

extern void shUpdateShCamera(ShEngine* p_engine, ShTransform* p_transform, ShCamera* p_camera);

extern void shSceneUpdate(ShEngine* p_engine, const uint32_t scene_idx);

//extern void shRenderMesh(const ShVkCore engine, const ShVkGraphicsPipeline pipeline, const uint32_t push_const_size, void* p_push_const, const uint32_t uniforms_size, void* p_uniforms, ShMesh* p_mesh);

extern void shSceneRelease(ShEngine* p_engine, const uint32_t scene_idx);

#endif//SH_SCENE_HANDLE_H