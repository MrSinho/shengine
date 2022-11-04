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

#include "sheditor/shEditor.h"


#define SH_EDITOR_THREAD_COUNT 1


int shEditorMain(ShEngine* p_engine) {
	char window_title[256];
	if (p_engine->window.title != NULL) {
		strcpy(window_title, p_engine->window.title);
	}
	else {
		strcpy(window_title, "shengine editor");
	}
	shWindowSetup(window_title, 1366, 768, &p_engine->window);
	shCreateInstance(&p_engine->core, "shengine editor", "shengine", 1, p_engine->window.instance_extension_count, p_engine->window.pp_instance_extensions);
	shWindowCreateSurface(p_engine);
	shSelectPhysicalDevice(&p_engine->core, VK_QUEUE_GRAPHICS_BIT | VK_QUEUE_COMPUTE_BIT);
	shSetLogicalDevice(&p_engine->core);
	shGetGraphicsQueue(&p_engine->core);
	shGetComputeQueue(&p_engine->core);
	shInitSwapchainData(&p_engine->core);
	shInitDepthData(&p_engine->core);
	shCreateRenderPass(&p_engine->core);
	shSetFramebuffers(&p_engine->core);
	shCreateGraphicsCommandBuffers(&p_engine->core, SH_EDITOR_THREAD_COUNT);
	shCreateComputeCommandBuffers(&p_engine->core, SH_EDITOR_THREAD_COUNT);
	shSetSyncObjects(&p_engine->core);

	shEngineManageState(p_engine, shSetEngineState(p_engine), 1);
	return 0;
}


#ifndef SH_APPLICATION_TARGET_TYPE_EXECUTABLE
int main() {
	ShEngine engine = { 0 };
	return shEditorMain(&engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
