#ifndef SH_SCENE_HANDLE_H
#define SH_SCENE_HANDLE_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include <stdint.h>



typedef struct ShEngine ShEngine;

typedef struct ShIdentity         ShIdentity;
typedef struct ShCamera           ShCamera;
typedef struct ShTransform        ShTransform;
typedef struct ShHostMemoryLinker ShHostMemoryLinker;

typedef struct ShSceneProperties ShSceneProperties;



extern uint8_t shSceneInit(
	ShEngine* p_engine
);

extern uint8_t shUpdateTransform(
	ShTransform* p_transform
);

extern uint8_t shUpdateCamera(
	ShEngine*    p_engine, 
	ShTransform* p_transform, 
	ShCamera*    p_camera
);

extern uint8_t shSceneUpdate(
	ShEngine* p_engine
);

extern uint8_t shEndScene(
	ShEngine* p_engine
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_SCENE_HANDLE_H