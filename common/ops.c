//
// Created by me on 1/25/20.
//
#include <string.h>
#include "ops.h"

char op_mnemonics[][4] = {
    "NOP",
    "ADD",
    "SUB",
    "AND",
    "ORR",
    "XOR",
    "NOT",
    "LSH",
    "ASH",
    "TCU",
    "TCS",
    "SET",
    "MOV",
    "LDW",
    "STW",
    "LDB",
    "STB",
};

InstructionType instructionTypes[] = {
        OP,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_RB_RC,
        OP_RA_IMM,
        OP_RA_RB,
        OP_RA_RB,
        OP_RA_RB,
        OP_RA_RB,
        OP_RA_RB,
};

char *op_name(Op op) {
    return op_mnemonics[op];
}

Op op_from(char *name) {
    for(int i = MIN_OP; i <= MAX_OP; i++) {
        if(strcmp(name, op_mnemonics[i]) == 0) {
            return (Op) i;
        }
    }
    return UNKNOWN_OP;
}

InstructionType type_of(Op op) {
    return instructionTypes[op];
}


