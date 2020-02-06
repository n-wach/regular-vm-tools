//
// Created by me on 1/25/20.
//
#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <stdlib.h>
#include "../common/ops.h"
#include "../common/regs.h"
#include "../common/instructions.h"

#define DEBUG
#ifdef DEBUG
    #define debug(str) printf("%s ", str)
    #define debugln(str) printf("%s\n", str)
#endif
#ifndef DEBUG
    #define debug(str)
    #define debugln(str)
#endif

#define fprintfOp(out, instruction, op) fprintf(out, #op)
#define fprintfOpRa(out, instruction, op) fprintf(out, #op " %s", \
    reg_name(instruction.op_ra.ra))
#define fprintfOpRaImm(out, instruction, op) fprintf(out, #op " %s, %x", \
    reg_name(instruction.op_ra_imm.ra), \
    instruction.op_ra_imm.imm)
#define fprintfOpRaRb(out, instruction, op) fprintf(out, #op " %s, %s", \
    reg_name(instruction.op_ra_rb.ra), \
    reg_name(instruction.op_ra_rb.rb))
#define fprintfOpRaRbImm(out, instruction, op) fprintf(out, #op " %s, %s, %x", \
    reg_name(instruction.op_ra_rb_imm.ra), \
    reg_name(instruction.op_ra_rb_imm.rb), \
    instruction.op_ra_rb_imm.imm)
#define fprintfOpRaRbRc(out, instruction, op) fprintf(out, #op " %s, %s, %s", \
    reg_name(instruction.op_ra_rb_rc.ra), \
    reg_name(instruction.op_ra_rb_rc.rb), \
    reg_name(instruction.op_ra_rb_rc.rc))
    
int decode_instruction(const Instruction i, FILE *out) {
    Op op = i.op.op;
    switch(op) {
        case NOP:
            fprintfOp(out, i, NOP);
            break;
        case ADD:
            fprintfOpRaRbRc(out, i, ADD);
            break;
        case SUB:
            fprintfOpRaRbRc(out, i, SUB);
            break;
        case AND:
            fprintfOpRaRbRc(out, i, AND);
            break;
        case ORR:
            fprintfOpRaRbRc(out, i, ORR);
            break;
        case XOR:
            fprintfOpRaRbRc(out, i, XOR);
            break;
        case NOT:
            fprintfOpRaRb(out, i, NOT);
            break;
        case LSH:
            fprintfOpRaRbRc(out, i, LSH);
            break;
        case ASH:
            fprintfOpRaRbRc(out, i, ASH);
            break;
        case TCU:
            fprintfOpRaRbRc(out, i, TCU);
            break;
        case TCS:
            fprintfOpRaRbRc(out, i, TCS);
            break;
        case SET:
            fprintfOpRaImm(out, i, SET);
            break;
        case MOV:
            fprintfOpRaRb(out, i, MOV);
            break;
        case LDW:
            fprintfOpRaRb(out, i, LDW);
            break;
        case STW:
            fprintfOpRaRb(out, i, STW);
            break;
        case LDB:
            fprintfOpRaRb(out, i, LDB);
            break;
        case STB:
            fprintfOpRaRb(out, i, STB);
            break;
        default:
            printf("Unknown Op %x", op);
            return -1;
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
        Instruction i;
        fread(&i, 4, 1, input);
        if(feof(input)) break;
        int status = decode_instruction(i, output);
        if(status == -1) {
            printf("Decode error %x %x %x %x", i.op_ra_rb_rc.op, 
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
