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
    while (1) {
        Instruction i;
        fread(&i, 4, 1, input);
        if(feof(input)) break;
        int status = decode_instruction(i, output);
        if(status == -1) {
            printf("Decode error 0x%x 0x%x 0x%x 0x%x", i.op_ra_rb_rc.op,
                   i.op_ra_rb_rc.ra, i.op_ra_rb_rc.rb, i.op_ra_rb_rc.rc);
            fclose(input);
            fclose(output);
            return -1;
        }
    }

    printf("Done.\n");

    fclose(input);
    fclose(output);
}

