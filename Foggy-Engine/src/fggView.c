#include "fggView.h"

#include <string.h>

void fggSetView(mat4 view) {

	vec3 center = { 0.0f, 0.0f, 0.0f };
	vec3 up = { 0.0f, 1.0f, 0.0f };
	vec3 eye = { 0.0f, 0.0f, 1.0f };
	
	glm_lookat(eye, center, up, view);
}