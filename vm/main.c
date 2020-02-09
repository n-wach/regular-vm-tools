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

    execFile(input);

    fclose(input);
}
