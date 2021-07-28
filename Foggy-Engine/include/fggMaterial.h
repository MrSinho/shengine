#ifndef FGG_MATERIAL_H
#define FGG_MATERIAL_H

#include "fggEcsImplementation.h"

#include "fggVkCore.h"
#include "fggVkPipelineData.h"
#include "fggExport.h"



typedef enum FggShaderStageFlagBits {

	FGG_SHADER_STAGE_VERTEX_BIT 	= VK_SHADER_STAGE_VERTEX_BIT,
    FGG_SHADER_STAGE_GEOMETRY_BIT 	= VK_SHADER_STAGE_GEOMETRY_BIT,
    FGG_SHADER_STAGE_FRAGMENT_BIT 	= VK_SHADER_STAGE_FRAGMENT_BIT

} FggShaderStageFlagBits;



typedef struct FggMaterial {

	const char* 			vertexShaderPath;
	const char* 			fragmentShaderPath;	

	FggVkPipelineData		pipelineData;

} FggMaterial;



EZ_ECS_MAKE_COMPONENT_DEFINITIONS(FggMaterial, 2)

#endif