#include "shView.h"

#include <string.h>

void shSetView(vec3 position, vec3 front, vec3 up, mat4 view) {

	vec3 center = { 0.0f, 0.0f, 0.0f };
	
	glm_vec3_add(position, front, center);
	glm_lookat(position, center, up, view);
}