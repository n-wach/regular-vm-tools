//
// Created by me on 2/8/20.
//

#include <stdio.h>
#include <stddef.h>
#include "../asm/asm.h"
#include "../disasm/disasm.h"
#include "../vm/vm.h"

int main(int argc, char **argv) {
    if(argc != 2) {
        printf("Usage: %s [input.asm]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "rb");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    FILE *bin = fopen("bin", "wb");
    if(bin == NULL) {
        printf("Invalid temporary bin file");
        return -1;
    }

    FILE *output = fopen("dis.asm", "w");
    if(output == NULL) {
        printf("Invalid dis.asm file");
        return -1;
    }


    printf("Assembling... ");
    assemble(input, bin);
    fclose(bin);
    printf("Done!\n");

    bin = fopen("bin", "rb");
    if(bin == NULL) {
        printf("Invalid temporary bin file");
        return -1;
    }

    printf("Disassembling... ");
    disassembleFile(bin, output);
    printf("Done!\n");
    rewind(bin);

    printf("Executing... ");
    execFile(bin);
    printf("Done!\n");

}