#ifndef FGG_MATERIAL_H
#define FGG_MATERIAL_H

#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggMaterialInfo.h"



typedef struct FggMaterial {

	FggMaterialInfo			mat_info;
	FggVkFixedStates		fixed_states;
	FggVkPipelineData		pipeline_data;

} FggMaterial;



FGG_ECS_MAKE_COMPONENT_DEFINITIONS(FggMaterial, 2)



extern void fggSetupMaterial(const FggVkCore core, const FggMaterialInfo mat_info, FggMaterial* p_material);

extern void fggCreateMaterialInstance(const FggVkCore core, FggMaterial* src, FggMaterial* dst);

#endif // FGG_MATERIAL_H