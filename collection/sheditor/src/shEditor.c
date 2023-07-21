#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>



#include <shengine/shEngine.h>
#include "sheditor/shEditor.h"




#define SH_ENGINE_RENDERPASS_ATTACHMENT_COUNT 3
#define SH_ENGINE_SWAPCHAIN_IMAGE_COUNT       2
#define SH_ENGINE_MAX_SWAPCHAIN_IMAGE_COUNT   6



int shEditorMain(ShEngine* p_engine) {

	char window_title[256];
	if (p_engine->window.title != NULL) {
		strcpy(window_title, p_engine->window.title);
	}
	else {
		strcpy(window_title, "shengine editor");
	}
	shWindowSetup(window_title, &p_engine->window);

	shEngineSetupVulkan(p_engine);

	shEngineManageState(p_engine, shSetEngineState(p_engine));

	return 0;
}


#ifndef SH_APPLICATION_TARGET_TYPE_EXECUTABLE
int main() {
	ShEngine* p_engine = shAllocateEngine();
	shEngineError(
		p_engine == NULL,
		"main: invalid engine memory",
		return -1
	);
	p_engine->load_shared = 1;
	return shEditorMain(p_engine);
}
#endif//SH_APPLICATION_TARGET_TYPE_EXECUTABLE

#ifdef __cplusplus
}
#endif//__cplusplus
