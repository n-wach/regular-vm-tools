//
// Created by me on 1/25/20.
//

#include "regs.h"
#include <string.h>

int MIN_REG = 0x00;
int MAX_REG = 0x1f;

char reg_mnemonics[][4] = {
        "R0",
        "R1",
        "R2",
        "R3",
        "R4",
        "R5",
        "R6",
        "R7",
        "R8",
        "R9",
        "R10",
        "R11",
        "R12",
        "R13",
        "R14",
        "R15",
        "R16",
        "R17",
        "R18",
        "R19",
        "R20",
        "R21",
        "R22",
        "R23",
        "R24",
        "R25",
        "R26",
        "R27",
        "R28",
        "R29",
        "R30",
        "R31",
};

char *reg_name(Reg reg) {
    return reg_mnemonics[reg];
}

Reg reg_from(char *name) {
    for(int i = MIN_REG; i <= MAX_REG; i++) {
        if(strcmp(name, reg_mnemonics[i]) == 0) {
            return (Reg) i;
        }
    }
    return UNKNOWN_REG;
}

