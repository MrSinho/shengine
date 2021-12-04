#include "fggProjection.h"

#include "fggWindow.h"

#include "memory.h"

void fggSetProjection(const FggWindow window, float fov, float nc, float fc, mat4 projection) {
	mat4 proj;
	glm_perspective(fov, (float)window.width / (float)window.height, nc, fc, proj);
	memcpy(projection, proj, sizeof(mat4));
}