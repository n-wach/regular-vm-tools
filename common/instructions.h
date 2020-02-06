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

#define genOpCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    return get_instructions(1, (int) op); \
}

#define genOpRaCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    Instruction* ins = get_instructions(1, (int) op); \
    ins->op_ra.ra = reg_from(s->params[0]); \
    return ins; \
}

#define genOpRaImmCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    Instruction* ins = get_instructions(1, (int) op); \
    ins->op_ra_imm.ra = reg_from(s->params[0]); \
    ins->op_ra_imm.imm = imm_from(prog, s->params[1]); \
    return ins; \
}

#define genOpRaRbCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    Instruction* ins = get_instructions(1, (int) op); \
    ins->op_ra_rb.ra = reg_from(s->params[0]); \
    ins->op_ra_rb.rb = reg_from(s->params[1]); \
    return ins; \
}

#define genOpRaRbImmCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    Instruction* ins = get_instructions(1, (int) op); \
    ins->op_ra_rb_imm.ra = reg_from(s->params[0]); \
    ins->op_ra_rb_imm.rb = reg_from(s->params[1]); \
    ins->op_ra_rb_imm.imm = imm_from(s->params[2]); \
    return ins; \
}


#define genOpRaRbRcCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    Instruction* ins = get_instructions(1, (int) op); \
    ins->op_ra_rb_rc.ra = reg_from(s->params[0]); \
    ins->op_ra_rb_rc.rb = reg_from(s->params[1]); \
    ins->op_ra_rb_rc.rc = reg_from(s->params[2]); \
    return ins; \
}

typedef union instruction {
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
