#include "fggProjection.h"

#include "fggWindow.h"

void fggSetProjection(const FggWindow window, float fov, float nc, float fc, mat4 projection) {
	
	glm_perspective(fov, (float)window.width / (float)window.height, nc, fc, projection);

}