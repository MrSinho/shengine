#include "fggExport.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fggComponents.h"

void fggExport(const char* path, const ezecsScene scene) {

    FILE* stream = fopen(path, "wb");
    uint32_t offset = 0;

    const uint32_t fggComponentIDs[EZ_ECS_MAX_COMPONENTS] = { ezecsFggTransformID, ezecsFggMeshID, ezecsFggMaterialID };
    const uint32_t fggComponentExportSizes[EZ_ECS_MAX_COMPONENTS] = { fggFggTransformExportSize, fggFggMeshExportSize, fggFggMaterialExportSize };
    
    for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
        fwrite(&fggComponentIDs[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
        fwrite(&fggComponentExportSizes[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
    }

    for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
        for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
            if (scene[entity][component] == NULL) {
                char null = '\0';
                uint32_t sz = 1;
                while (sz < fggComponentExportSizes[component]) {
                    fwrite(&null, 1, 1, stream);
                    offset++; 
                    sz++;
                    fseek(stream, offset, SEEK_SET);
                }
            }
            else {
                fwrite(scene[entity][component], fggComponentExportSizes[component], 1, stream);
                offset += fggComponentExportSizes[component];
                fseek(stream, offset, SEEK_SET);
            }
        }
    }

    fclose(stream);
}   