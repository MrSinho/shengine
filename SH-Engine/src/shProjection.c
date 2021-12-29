#include "shProjection.h"
#include "shCglmImplementation.h"
#include "shWindow.h"
#include "memory.h"

void shSetProjection(const ShWindow window, float fov, float nc, float fc, shmat4 projection) {
	shmat4 proj;
	glm_perspective(fov, (float)window.width / (float)window.height, nc, fc, proj);
	memcpy(projection, proj, sizeof(shmat4));
}