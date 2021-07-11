#ifndef FGG_MATERIAL_H
#define FGG_MATERIAL_H

#include <fggEcsImplementation.h>

#include "fggVkPipelineData.h"

typedef struct FggMaterial {

	FggVkPipelineData pipelineData;

} FggMaterial;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMaterial, 4)

#endif