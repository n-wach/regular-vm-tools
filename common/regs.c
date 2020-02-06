//
// Created by me on 2/5/20.
//

#include <stdint.h>
#include <string.h>
#include "../asm/statement.h"
#include "../calc/calc.h"
#include "regs.h"

char reg_mnemonics[][4] = {
        "pc",
        "r1",
        "r2",
        "r3",
        "r4",
        "r5",
        "r6",
        "r7",
        "r8",
        "r9",
        "r10",
        "r11",
        "r12",
        "r13",
        "r14",
        "r15",
        "r16",
        "r17",
        "r18",
        "r19",
        "r20",
        "r21",
        "r22",
        "r23",
        "r24",
        "r25",
        "r26",
        "r27",
        "r28",
        "at0",
        "at1",
        "sp"
};

uint16_t imm_from(StatementList *prog, char *exp) {
    return eval(prog, exp);
}

char *reg_name(Reg reg) {
    return reg_mnemonics[reg];
}

Reg reg_from(char *name) {
    if(name == NULL) {
        printf("Missing register\n");
        return UNKNOWN_REG;
    }
    for(int i = MIN_REG; i <= MAX_REG; i++) {
        if(strcmp(name, &reg_mnemonics[i][0]) == 0) {
            return (Reg) i;
        }
    }
    printf("Unknown register %s\n", name);
    return UNKNOWN_REG;
}

