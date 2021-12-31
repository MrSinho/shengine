#ifndef SH_MATERIAL_H
#define SH_MATERIAL_H

#include "shVkCore.h"
#include "shVkPipelineData.h"
#include "shMaterialInfo.h"



typedef struct ShMaterial {

	ShMaterialInfo			mat_info;
	ShVkFixedStates			fixed_states;
	ShVkPipelineData		pipeline_data;

} ShMaterial;



SH_ECS_MAKE_COMPONENT_DEFINITIONS(ShMaterial, 2)



extern void shSetupMaterial(const ShVkCore core, const ShMaterialInfo mat_info, ShMaterial* p_material);

#endif//SH_MATERIAL_H