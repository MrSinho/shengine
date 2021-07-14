#ifndef FGG_MATERIAL_H
#define FGG_MATERIAL_H

#include <fggEcsImplementation.h>

#include "fggVkCore.h"
#include "fggVkPipelineData.h"

typedef struct FggMaterial {

	FggVkPipelineData		pipelineData;

	VkPushConstantRange		pushConstantRange;
	void*					pPushConstantsData;

} FggMaterial;

EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMaterial, 4)

#endif