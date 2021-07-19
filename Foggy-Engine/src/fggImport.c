#include "fggImport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fggComponents.h"

void fggImport(const char* path, ezecsScene scene) {
    
    FILE* stream = fopen(path, "rb");
    uint32_t offset = 0;

    const uint32_t fggComponentIDs[EZ_ECS_MAX_COMPONENTS] = { ezecsFggTransformID, ezecsFggMeshID, ezecsFggMaterialID };
    const uint32_t fggComponentExportSizes[EZ_ECS_MAX_COMPONENTS] = { fggFggTransformExportSize, fggFggMeshExportSize, fggFggMaterialExportSize };



    uint32_t componentIDs[EZ_ECS_MAX_COMPONENTS] = { 0 };
    uint32_t componentSizes[EZ_ECS_MAX_COMPONENTS] = { 0 };

    for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
        fread(&componentIDs[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
        fread(&componentSizes[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
    }

    for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
        for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
            scene[entity][component] = calloc(1, componentSizes[component]);
            if (scene[entity][component] == NULL) { return; }
            fread(scene[entity][component], componentSizes[component], 1, stream);
            offset += componentSizes[component];
            fseek(stream, offset, SEEK_SET);
        }
    }

    FggTransform*   trans   = ezecsGetFggTransform(scene, 0);
    FggMesh*        mesh    = ezecsGetFggMesh(scene, 0);
    FggMaterial*    mat     = ezecsGetFggMaterial(scene, 0);

    fclose(stream);
}