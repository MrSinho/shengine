#ifndef SH_PROJECTION_H
#define SH_PROJECTION_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

typedef struct ShWindow ShWindow;

extern void shSetProjection(const ShWindow window, float fov, float nc, float fc, float projection[4][4]);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif