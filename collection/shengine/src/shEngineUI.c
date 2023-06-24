#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



#include "shengine/shEngineUI.h"
#include "shengine/shEngine.h"

#include <shgui/shgui.h>



uint8_t shEngineGuiSetup(
	ShEngine* p_engine
) {
    shEngineError(p_engine == NULL, "shEngineGuiSetup: invalid engine memory", return 0);

	p_engine->p_gui = shAllocateGui();
	shEngineError(
		p_engine->p_gui == NULL, 
		"shEngineGuiSetup: invalid gui memory", 
		return 0
	);

	ShGuiCore gui_core = {
		p_engine->core.device,                      //device
		p_engine->core.physical_device,             //physical_device
		p_engine->core.graphics_queue_family_index, //graphics_queue_family_index
		p_engine->core.graphics_queue,              //graphics_queue
		p_engine->core.surface,                     //surface
		p_engine->core.renderpass,                  //render_pass
		p_engine->core.sample_count,                //sample_count
		p_engine->core.swapchain_image_count        //swapchain_image_count
	};
	shGuiInit(p_engine->p_gui, gui_core);

	shGuiAllocateMemory(p_engine->p_gui);

	shGuiAllocatePipelineResources(p_engine->p_gui, p_engine->core.swapchain_image_count);

	shGuiBuildRegionPipeline(
		p_engine->p_gui,
		NULL,
		NULL
	);
	shGuiBuildCharPipeline(
		p_engine->p_gui,
		NULL,
		NULL
	);

	shGuiLinkInputs(
		p_engine->p_gui,
		&p_engine->window.width, &p_engine->window.height,
		&p_engine->window.input.cursor_pos_x, &p_engine->window.input.cursor_pos_y,
		p_engine->window.input.key_events, p_engine->window.input.mouse_events,
		&p_engine->time.delta_time
	);

	shGuiScaleUI(p_engine->p_gui, 1.0f, 1.0f);

	return 1;
}


#ifdef __cplusplus
}
#endif//__cplusplus
