#ifndef SH_ENGINE_GUI_H
#define SH_ENGINE_GUI_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include "shengine/shEngine.h"

typedef enum ShGuiDefaultValues ShGuiDefaultValues;

extern uint8_t shEngineGuiSetup(ShEngine* p_engine, const uint32_t max_gui_item, const ShGuiDefaultValues default_values);



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_ENGINE_GUI_H
