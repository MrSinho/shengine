#include "FDL.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void fdlBuild(const char* path) {

    FILE* stream = fopen(path, "r");

    char line[256];
    while (fgets(line, 256, stream) != NULL) {
        
    }
}