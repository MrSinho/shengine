#include "shProjection.h"

#include "shWindow.h"

#include "memory.h"

void shSetProjection(const ShWindow window, float fov, float nc, float fc, mat4 projection) {
	mat4 proj;
	glm_perspective(fov, (float)window.width / (float)window.height, nc, fc, proj);
	memcpy(projection, proj, sizeof(mat4));
}