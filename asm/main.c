//
// Created by me on 2/6/20.
//
#include <stdio.h>
#include "asm.h"

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s [input] [output]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    FILE *output = fopen(argv[2], "wb");
    if(output == NULL) {
        printf("Invalid output file");
        fclose(input);
        return -1;
    }

    printf("Assembling...\n");
    assemble(input, output);
    printf("Done!\n");
    
    fclose(input);
    fclose(output);
}
