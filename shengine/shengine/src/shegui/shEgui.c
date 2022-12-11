#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shegui/shEgui.h"
#include "shengine/shEngine.h"

#include <shgui/shgui.h>



ShEngineStatus shEngineGuiSetup(ShEngine* p_engine, const uint32_t max_regions, const uint32_t max_chars, const ShGuiDefaultValues default_values) {
    shEngineError(p_engine == NULL, "invalid engine memory", return SH_ENGINE_INVALID_ENGINE_MEMORY);

    ShGuiCore gui_core = {
        p_engine->core.device,
        p_engine->core.physical_device,
        p_engine->core.graphics_queue,
        p_engine->core.p_graphics_commands[0].cmd_buffer,
        p_engine->core.p_graphics_commands[0].fence,
        p_engine->core.surface.surface,
        p_engine->core.render_pass
    };
    if (p_engine->p_gui != NULL) {
        return 1;
    }

    p_engine->p_gui = shGuiInit(gui_core);

    shGuiLinkInputs(
        &p_engine->window.width,
        &p_engine->window.height,
        &p_engine->window.input.cursor_pos_x,
        &p_engine->window.input.cursor_pos_y,
        p_engine->window.input.key_events,
        p_engine->window.input.mouse_events,
        p_engine->window.default_cursor_icons,
        &p_engine->time.delta_time,
        p_engine->p_gui
    );

    shGuiBuildRegionPipeline(p_engine->p_gui, NULL, NULL, max_regions);
    shGuiBuildTextPipeline(p_engine->p_gui, NULL, NULL, max_chars);
    shGuiSetDefaultValues(
        p_engine->p_gui,
        default_values, SH_GUI_RECORD | SH_GUI_INITIALIZE
    );

	return 1;
}


#ifdef __cplusplus
}
#endif//__cplusplus
