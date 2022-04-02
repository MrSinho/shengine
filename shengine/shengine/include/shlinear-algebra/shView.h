#ifndef SH_VIEW_H
#define SH_VIEW_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shCglmImplementation.h"

extern void shSetView(float* position, float* front, float* up, float view[4][4]);

#ifdef __cplusplus
}
#endif//__cplusplus

#endif//SH_VIEW_H