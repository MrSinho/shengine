#ifndef SH_IDENTITY_H
#define SH_IDENTITY_H

#ifdef __cplusplus
extern "C" {
#endif//__cplusplus

#include "shEcsImplementation.h"

typedef struct ShIdentity {
	const char* name;
	const char* tag;
	const char* subtag;
} ShIdentity;
SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShIdentity, 6)

#ifdef __cplusplus
}
#endif//__cplusplus

#endif // SH_IDENTITY_H