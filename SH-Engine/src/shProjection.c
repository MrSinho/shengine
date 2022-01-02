#include "shProjection.h"
#include "shCglmImplementation.h"
#include "shWindow.h"

#include <assert.h>
#include <memory.h>


void shSetProjection(const ShWindow window, float fov, float nc, float fc, float projection[4][4]) {
	assert(projection != NULL);
	float proj[4][4];
	glm_perspective(fov, (float)window.width / (float)window.height, nc, fc, proj);
	memcpy(projection, proj, 64);
}