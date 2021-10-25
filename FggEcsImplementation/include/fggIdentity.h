#ifndef FGG_IDENTITY_H
#define FGG_IDENTITY_H

#include "fggEcsImplementation.h"

typedef struct FggIdentity {

	const char* name;
	const char* tag;
	const char* subtag;

} FggIdentity;

FGG_ECS_MAKE_COMPONENT_DEFINITIONS(FggIdentity, 6)

#endif // FGG_IDENTITY_H