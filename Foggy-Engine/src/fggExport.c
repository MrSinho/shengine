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
    const uint32_t fggComponentExportSizes[EZ_ECS_MAX_COMPONENTS] = { ezecsFggTransformSize, ezecsFggMeshSize, ezecsFggMaterialSize};
    
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
                uint32_t max = UINT32_MAX;
                fwrite(&max, 1, sizeof(uint32_t), stream);
                offset += sizeof(uint32_t); 
                fseek(stream, offset, SEEK_SET);
            }
            else {
               
                if (component == ezecsFggMeshID) {
                    FggMesh* mesh = (FggMesh*)scene[entity][component];
                    fwrite(&mesh->vertexCount, sizeof(uint32_t), 1, stream);
                    offset += sizeof(uint32_t);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(mesh->pVertices, sizeof(float), mesh->vertexCount, stream);
                    offset += mesh->vertexCount * sizeof(float);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(&mesh->indexCount, sizeof(uint32_t), 1, stream);
                    offset += sizeof(uint32_t);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(mesh->pIndices, sizeof(uint32_t), mesh->indexCount, stream);
                    offset += mesh->indexCount * sizeof(uint32_t);
                    fseek(stream, offset, SEEK_SET);
                }

                if (component == ezecsFggTransformID) {
                    FggTransform* trans = (FggTransform*)scene[entity][component];
                    fwrite(trans->model, sizeof(mat4), 1, stream);
                    offset += sizeof(mat4);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->position, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->rotation, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->scale, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->front, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->left, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                    fwrite(trans->up, sizeof(vec3), 1, stream);
                    offset += sizeof(vec3);
                    fseek(stream, offset, SEEK_SET);
                }
            }
        }
    }

    fclose(stream);
}   