#include "fggProjection.h"

#include "fggWindow.h"

void fggProjectionUpdate(const FggWindow window, FggProjection* meshPushConstants) {
	
	glm_perspective(45.0f, (float)window.width / (float)window.height, 0.1f, 150.0f, meshPushConstants->projection);

}