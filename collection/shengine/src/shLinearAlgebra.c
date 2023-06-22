#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shengine/shLinearAlgebra.h"

#include "shengine/shEngine.h"
#include "shengine/shWindow.h"

#include <memory.h>



uint8_t shSetProjection(
	ShWindow window,
	float    fov,
	float    nc,
	float    fc,
	float    projection[4][4]
) {
	shEngineError(window.width  == 0, "shSetProjection: invalind window width",  return 0);
	shEngineError(window.height == 0, "shSetProjection: invalind window height", return 0);

	glm_perspective(
		SH_DEGREES_TO_RADIANS(fov),
		(float)window.width / (float)window.height,
		nc,
		fc,
		projection
	);

	return 1;
}

uint8_t shSetView(
	float* p_position,
	float* p_front,
	float* p_up,
	float  view[4][4]
) {
	shEngineError(p_position == NULL, "shSetView: invalid position memory", return 0);
	shEngineError(p_front    == NULL, "shSetView: invalid front memory",    return 0);
	shEngineError(p_up       == NULL, "shSetView: invalid up memory",       return 0);

	float center[3] = { 0.0f, 0.0f, 0.0f };

	glm_vec3_add(p_position, p_front, center);
	glm_lookat(p_position, center, p_up, view);

	return 1;
}


#ifdef __cplusplus
}
#endif//__cplusplus