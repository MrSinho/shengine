#include "fggPushConstants.h"

#include "fggWindow.h"

void fggPushConstantsUpdate(const FggWindow window, FggMeshPushConstants* meshPushConstants) {
	
	glm_perspective(45.0f, (float)window.width / (float)window.height, 0.1f, 150.0f, meshPushConstants->projection);

}