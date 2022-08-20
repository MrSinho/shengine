#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shlinear-algebra/shProjection.h"
#include "shlinear-algebra/shCglmImplementation.h"
#include "shlinear-algebra/shEuler.h"

#include "shengine/shWindow.h"

#include <assert.h>
#include <memory.h>


void shSetProjection(const ShWindow window, float fov, float nc, float fc, float projection[4][4]) {
	assert(projection != NULL);
	float proj[4][4];
	glm_perspective(SH_DEGREES_TO_RADIANS(fov), (float)window.width / (float)window.height, nc, fc, proj);
	memcpy(projection, proj, 64);
}

#ifdef __cplusplus
}
#endif//__cplusplus