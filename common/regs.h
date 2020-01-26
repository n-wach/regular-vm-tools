//
// Created by me on 1/25/20.
//

#ifndef ASSEMBLER_REGS_H
#define ASSEMBLER_REGS_H

#define MIN_REG 0x0
#define MAX_REG 0x1f

typedef enum reg {
    R0=0x00,
    R1=0x01,
    R2=0x02,
    R3=0x03,
    R4=0x04,
    R5=0x05,
    R6=0x06,
    R7=0x07,
    R8=0x08,
    R9=0x09,
    R10=0x0a,
    R11=0x0b,
    R12=0x0c,
    R13=0x0d,
    R14=0x0e,
    R15=0x0f,
    R16=0x10,
    R17=0x11,
    R18=0x12,
    R19=0x13,
    R20=0x14,
    R21=0x15,
    R22=0x16,
    R23=0x17,
    R24=0x18,
    R25=0x19,
    R26=0x1a,
    R27=0x1b,
    R28=0x1c,
    R29=0x1d,
    R30=0x1e,
    R31=0x1f,
    UNKNOWN_REG=0xff,
} Reg;

extern char reg_mnemonics[][4];

char *reg_name(Reg reg);

Reg reg_from(char *name);

#endif //ASSEMBLER_REGS_H
