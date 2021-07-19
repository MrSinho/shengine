#include "fggImport.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fggComponents.h"

void fggImport(const char* path, ezecsScene scene) {
    
    FILE* stream = fopen(path, "rb");
    uint32_t offset = 0;

    uint32_t fggComponentIDs[EZ_ECS_MAX_COMPONENTS] = { 0 };
    uint32_t fggComponentSizes[EZ_ECS_MAX_COMPONENTS] = { 0 };

    for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
        fread(&fggComponentIDs[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
        fread(&fggComponentSizes[component], sizeof(uint32_t), 1, stream);
        offset += sizeof(uint32_t);
        fseek(stream, offset, SEEK_SET);
    }

    for (uint32_t entity = 0; entity < EZ_ECS_MAX_ENTITIES; entity++) {
        for (uint32_t component = 0; component < EZ_ECS_MAX_COMPONENTS; component++) {
            uint32_t valid = 1;
            fread(&valid, sizeof(uint32_t), 1, stream);
            fseek(stream, offset, SEEK_SET);
            if (valid != UINT32_MAX) {
                scene[entity][component] = calloc(1, fggComponentSizes[component]);
                if (scene[entity][component] == NULL) { return; }
                if (component == ezecsFggMeshID) {
                    FggMesh* mesh = (FggMesh*)scene[entity][component];
                    fread(&mesh->vertexCount, sizeof(uint32_t), 1, stream);
                    offset += sizeof(uint32_t);
                    fseek(stream, offset, SEEK_SET);
                    mesh->pVertices = calloc(mesh->vertexCount, sizeof(float));
                    if (mesh->pVertices != NULL) {
                        fread(mesh->pVertices, sizeof(float), mesh->vertexCount, stream);
                        offset += mesh->vertexCount * sizeof(float);
                        fseek(stream, offset, SEEK_SET);
                    }
                    fread(&mesh->indexCount, sizeof(uint32_t), 1, stream);
                    offset += sizeof(uint32_t);
                    fseek(stream, offset, SEEK_SET);
                    mesh->pIndices = calloc(mesh->indexCount, sizeof(uint32_t));
                    if (mesh->pIndices != NULL) {
                        fread(mesh->pIndices, sizeof(uint32_t), mesh->indexCount, stream);
                        offset += mesh->indexCount * sizeof(uint32_t);
                        fseek(stream, offset, SEEK_SET);
                    }
                }
            }
        }
    }

    FggTransform*   trans   = ezecsGetFggTransform(scene, 0);
    FggMesh*        mesh    = ezecsGetFggMesh(scene, 0);
    FggMaterial*    mat     = ezecsGetFggMaterial(scene, 0);

    fclose(stream);
}