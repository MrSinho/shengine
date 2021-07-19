#ifndef FGG_COMPONENTS_H
#define FGG_COMPOENTS_H

#include "fggEcsImplementation.h"
#include "fggTransform.h"
#include "fggMesh.h"
#include "fggMaterial.h"

static const uint32_t fggComponentIDs = {
    ezecsFggTransformID, ezecsFggMeshID, ezecsFggMaterialID
};

static const uint32_t fggComponentSizes = {
    ezecsFggTransformSize, ezecsFggMeshSize, ezecsFggMaterialSize
};

#endif