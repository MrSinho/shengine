#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



#include <shsharedhost/shSharedHost.h>
#include <shfd/shFd.h>

#include <shengine/shEngine.h>

#include <shvulkan/shVkCore.h>
#include <shvulkan/shVkMemoryInfo.h>
#include <shvulkan/shVkDrawLoop.h>

#include <shgui/shgui.h>


#define SH_EDITOR_THREAD_COUNT 1



int main() {

	ShEngine engine = { 0 };
	shWindowSetup("shengine editor", 720, 480, &engine.window);
	shCreateInstance(&engine.core, "shengine editor", "shengine", 1, engine.window.instance_extension_count, engine.window.pp_instance_extensions);
	shWindowCreateSurface(&engine);
	shSelectPhysicalDevice(&engine.core, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	shSetLogicalDevice(&engine.core);
	shGetGraphicsQueue(&engine.core);
	shGetComputeQueue(&engine.core);
	shInitSwapchainData(&engine.core);
	shInitDepthData(&engine.core);
	shCreateRenderPass(&engine.core);
	shSetFramebuffers(&engine.core);
	shCreateGraphicsCommandBuffers(&engine.core, SH_EDITOR_THREAD_COUNT);
	shCreateComputeCommandBuffers(&engine.core, SH_EDITOR_THREAD_COUNT);
	shSetSyncObjects(&engine.core);

	ShGuiCore gui_core = {
		engine.core.device,
		engine.core.physical_device,
		engine.core.graphics_queue,
		engine.core.p_graphics_commands[0].cmd_buffer,
		engine.core.p_graphics_commands[0].fence,
		engine.core.surface.surface
	};
	engine.p_gui = shGuiInit(gui_core);
	
	shGuiLinkInputs(
		&engine.window.width, 
		&engine.window.height,
		NULL,
		NULL,
		engine.window.input.key_events,
		engine.window.input.mouse_events,
		(ShGuiCursorIcons) { GLFW_CURSOR_NORMAL, GLFW_HRESIZE_CURSOR, GLFW_VRESIZE_CURSOR },
		(float*)&engine.time.delta_time,
		engine.p_gui
	);
	shGuiBuildRegionPipeline(engine.p_gui, engine.core.render_pass, 256);
	shGuiBuildTextPipeline(engine.p_gui, engine.core.render_pass, 256);
	shGuiSetDefaultValues(engine.p_gui, SH_GUI_THEME_DARK, SH_GUI_RECORD | SH_GUI_INITIALIZE);

	shMakeAssetsPath("/descriptors/materials.json", engine.materials_descriptor.path);
	shMakeAssetsPath("/descriptors/scene.json", engine.scene_descriptor.path);
	shMakeAssetsPath("/descriptors/simulation.json", engine.simulation_descriptor.path);

	shEngineManageState(&engine, shSetEngineState(&engine));

	return 0;
}

#ifdef __cplusplus
}
#endif//__cplusplus
