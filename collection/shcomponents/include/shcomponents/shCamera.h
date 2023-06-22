#ifndef SH_CAMERA_H
#define SH_CAMERA_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus


#include <stdint.h>



#define SH_IDENTITY_STR256_LENGTH 32
#define SH_CAMERA_FLAG_COUNT      2



typedef struct ShCamera {
	float	fov;
	float	nc;
	float	fc;
	float 	speed;
	float 	mouse_speed;
	float	projection[4][4];
	float	view[4][4];
	uint8_t free_flight;
	uint8_t active;
} ShCamera;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_CAMERA_H