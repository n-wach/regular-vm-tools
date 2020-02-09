//
// Created by me on 2/5/20.
//

#include <stdio.h>
#include "../common/instructions.h"
#include "disasm.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s [input] [output]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "rb");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    FILE *output = fopen(argv[2], "w");
    if(output == NULL) {
        printf("Invalid output file");
        fclose(input);
        return -1;
    }
    
    printf("Reading...\n");
    decompileFile(input, output);
    printf("Done.\n");

    fclose(input);
    fclose(output);
}
