#include "fggProjection.h"

#include "fggWindow.h"



void fggSetProjection(const FggWindow window, mat4 projection) {
	
	glm_perspective(45.0f, (float)window.width / (float)window.height, 0.1f, 150.0f, projection);

}