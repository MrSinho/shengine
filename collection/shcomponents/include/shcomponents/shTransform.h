#ifndef SH_TRANSFORM_H
#define SH_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



typedef struct ShTransform {
	float   model[4][4];
	float   position[3];
	float   rotation[3];
	float   euler[3];
	float   scale[3];
	float   front[3];
	float   left[3];
	float   up[3];
	uint8_t active;
} ShTransform;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TRANSFORM_H