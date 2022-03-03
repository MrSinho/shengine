#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "linear_algebra/shView.h"

#include <assert.h>

void shSetView(float* position, float* front, float* up, float view[4][4]) {
	assert(position != NULL && front != NULL && up != NULL && view != NULL);
	float center[3] = { 0.0f, 0.0f, 0.0f };
	glm_vec3_add(position, front, center);
	glm_lookat(position, center, up, view);
}

#ifdef __cplusplus
}
#endif//__cplusplus