#ifndef SH_ENGINE_GUI_H
#define SH_ENGINE_GUI_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "shengine/shEngine.h"


extern uint8_t shEngineGuiSetup(
	ShEngine* p_engine, 
	uint32_t  max_regions,
	uint32_t  max_chars 
);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_GUI_H
