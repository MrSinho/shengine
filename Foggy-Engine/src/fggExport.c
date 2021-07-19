#include "fggExport.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include "fggComponents.h"

void fggExport(const char* path) {

    FILE* stream = fopen(path, "wb");
    uint32_t offset = 0;

    const uint32_t maxEntities = EZ_ECS_MAX_ENTITIES;
    fwrite(&maxEntities, sizeof(uint32_t), 1, stream);
    offset += sizeof(uint32_t);
    fseek(stream, offset, SEEK_SET);

    const uint32_t maxComponents = EZ_ECS_MAX_COMPONENTS;
    fwrite(&maxComponents, sizeof(uint32_t), 1, stream);
    offset += sizeof(uint32_t);
    fseek(stream, offset, SEEK_SET);

    fwrite(fggComponentIDs, sizeof(uint32_t), EZ_ECS_MAX_COMPONENTS, stream);
    fwrite(fggComponentSizes, sizeof(uint32_t), EZ_ECS_MAX_COMPONENTS, stream);
    offset += 2 * EZ_ECS_MAX_COMPONENTS * sizeof(uint32_t);
    fseek(stream, offset, SEEK_SET);
    
    //oh shit dunno how to go forwards
}   