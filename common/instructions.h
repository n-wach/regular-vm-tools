//
// Created by me on 2/4/20.
//

#ifndef REGULARVM_INSTRUCTIONS_H
#define REGULARVM_INSTRUCTIONS_H

#include <stdarg.h>
#include <malloc.h>
#include <stdint.h>
#include "ops.h"
#include "regs.h"

typedef union instruction {
    uint32_t ui32;
    struct {
        Op op;
    } op;
    struct {
        Op op;
        Reg ra;
    } op_ra;
    struct {
        Op op;
        Reg ra;
        uint16_t imm;
    } op_ra_imm;
    struct {
        Op op;
        Reg ra;
        Reg rb;
    } op_ra_rb;
    struct {
        Op op;
        Reg ra;
        Reg rb;
        uint8_t imm;
    } op_ra_rb_imm;
    struct {
        Op op;
        Reg ra;
        Reg rb;
        Reg rc;
    } op_ra_rb_rc;
} Instruction;

#endif //REGULARVM_INSTRUCTIONS_H
