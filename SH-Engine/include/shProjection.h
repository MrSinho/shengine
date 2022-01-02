#ifndef SH_PROJECTION_H
#define SH_PROJECTION_H

typedef struct ShWindow ShWindow;

extern void shSetProjection(const ShWindow window, float fov, float nc, float fc, float projection[4][4]);

#endif