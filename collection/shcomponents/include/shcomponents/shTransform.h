#ifndef SH_TRANSFORM_H
#define SH_TRANSFORM_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus



typedef struct ShTransform {
	float   model[4][4];
	float   position[4];
	float   rotation[4];
	float   euler[4];
	float   scale[4];
	float   front[4];
	float   left[4];
	float   up[4];
} ShTransform;



#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_TRANSFORM_H