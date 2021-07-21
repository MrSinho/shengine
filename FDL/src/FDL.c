#include "FDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#pragma warning (disable: 6387)

void fdlBuild(const char* path) {

    FILE* stream = fopen(path, "r");
    if (stream == NULL) { return; }

    char line[256];

    uint32_t materials = 0;
    uint32_t offset = 0;

    while (fgets(line, 256, stream) != NULL) {
        
        //ENTITY
        if (line[0] == 'e' && line[1] == 'n' && line[2] == 't') {
            fgets(line, 256, stream);

            //MESH
            if (line[1] == 'm' && line[2] == 'e' && line[3] == 's') {
                fgets(line, 256, stream);
                
                //MESH PATH
                if (line[2] == 'p' && line[3] == 'a' && line[4] == 't') {
                    char meshPath[256];
                    fgets(line, 256, stream);
                    line[strlen(line) - 1] = '\0';
                    memcpy(meshPath, &line[3], strlen(line));
                    printf(meshPath);
                    fgets(line, 256, stream);
                }
            }

            //TRANSFORM 
            if (line[1] == 't' && line[2] == 'r' && line[3] == 'a') {
                fgets(line, 256, stream);

                //POSITION
                if (line[2] == 'p') {
                    fgets(line, 256, stream);
                    char pos[256];
                    memcpy(pos, &line[3], strlen(line));
                    printf(pos);
                    fgets(line, 256, stream);
                }

                //ROTATION
                if (line[2] == 'r') {
                    fgets(line, 256, stream);
                    char rot[256];
                    memcpy(rot, &line[3], strlen(line));
                    printf(rot);
                    fgets(line, 256, stream);
                }

                //SCALE
                if (line[2] == 's') {
                    fgets(line, 256, stream);
                    char scale[256];
                    memcpy(scale, &line[3], strlen(line));
                    printf(scale);
                    fgets(line, 256, stream);
                }
            }

            //MATERIAL
            if (line[1] == 'm' && line[2] == 'a' && line[3] == 't') {
                fgets(line, 256, stream);
                if (line[2] == 'v') {
                    fgets(line, 256, stream);
                    line[strlen(line) - 1] = '\0';
                    char vpath[256];
                    strcpy(vpath, &line[3]);
                    fgets(line, 256, stream);
                }
                if (line[2] == 'f') {
                    fgets(line, 256, stream);
                    line[strlen(line) - 1] = '\0';
                    char fpath[256];
                    strcpy(fpath, &line[3]);
                    fgets(line, 256, stream);
                }
            }
        }
    }
}