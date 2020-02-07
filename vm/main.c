//
// Created by me on 2/6/20.
//
#include <stdio.h>
#include "vm.h"

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [bin]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "rb");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    fseek (input , 0 , SEEK_END);
    size_t fileSize = ftell(input);
    rewind(input);

    if(fileSize > MEM_SIZE) {
        printf("Program too large for memory\n");
        return -1;
    }

    VM vm;
    vmInit(&vm);

    size_t result = fread(vm.memory, 1, fileSize, input);
    if (result != fileSize) {
        printf("Failed to read program into memory\n");
        return -1;
    }

    vmRun(&vm);

    vmFree(&vm);
    fclose(input);
}
