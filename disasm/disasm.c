//
// Created by me on 1/25/20.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "../common/ops.h"
#include "../common/regs.h"

#define DEBUG
#ifdef DEBUG
    #define debug(str) printf("%s ", str)
    #define debugln(str) printf("%s\n", str)
#endif
#ifndef DEBUG
    #define debug(str)
    #define debugln(str)
#endif


int decode_instruction(const char instr[4], FILE *output) {
    Op op = (Op) instr[0];
    Reg ra;
    Reg rb;
    Reg rc;
    int imm;

    if(op < MIN_OP || op > MAX_OP) {
        debugln("Unknown OP");
        return -1;
    } else {
        InstructionType type = type_of(op);
        switch(type) {
            case OP:
                fprintf(output, "%s\n", op_mnemonics[op]);
                break;
            case OP_RA:
                ra = instr[1];
                fprintf(output, "%s %s\n", op_mnemonics[op], reg_mnemonics[op]);
                break;
            case OP_RA_IMM:
                ra = instr[1];
                imm = (instr[2] << 8) | instr[3];
                fprintf(output, "%s %s 0x%x\n", op_mnemonics[op], reg_mnemonics[ra], imm);
                break;
            case OP_RA_RB:
                ra = instr[1];
                rb = instr[2];
                fprintf(output, "%s %s %s\n", op_mnemonics[op], reg_mnemonics[ra], reg_mnemonics[rb]);
                break;
            case OP_RA_RB_IMM:
                ra = instr[1];
                rb = instr[2];
                imm = instr[3];
                fprintf(output, "%s %s %s 0x%x\n", op_mnemonics[op], reg_mnemonics[ra], reg_mnemonics[rb], imm);
                break;
            case OP_RA_RB_RC:
                ra = instr[1];
                rb = instr[2];
                rc = instr[3];
                fprintf(output, "%s %s %s %s\n", op_mnemonics[op], reg_mnemonics[ra], reg_mnemonics[rb], reg_mnemonics[rc]);
                break;
            default:
                debugln("UNKNOWN TYPE");
                return -1;
        }
    }

    return 1;
}


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
        char instr[4];
        fread(&instr, 1, 4, input);
        if(feof(input)) break;
        int status = decode_instruction(instr, output);
        if(status == -1) {
            printf("Decode error %x %x %x %x", instr[0], instr[1], instr[2], instr[3]);
            fclose(input);
            fclose(output);
            return -1;
        }
    }

    printf("Done.\n");

    fclose(input);
    fclose(output);
}
