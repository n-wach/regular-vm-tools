//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_COMPILER_H
#define REGULARVM_COMPILER_H

#include <stddef.h>
#include "../common/instructions.h"
#include "statement.h"
#include "../common/regs.h"

int imm_from(StatementList *prog, char *exp);

#define init_instrs(count) \
    Instruction *instructions = malloc(sizeof(Instruction) * (count)); \
    int c = 0;
#define put_op(_op) \
    instructions[c].op.op = _op; c++;
#define put_op_ra(_op, _ra) \
    instructions[c].op_ra.op = _op; \
    instructions[c].op_ra.ra = _ra; c++;
#define put_op_ra_imm(_op, _ra, _imm) \
    instructions[c].op_ra_imm.op = _op; \
    instructions[c].op_ra_imm.ra = _ra; \
    instructions[c].op_ra_imm.imm = _imm; c++;
#define put_op_ra_rb(_op, _ra, _rb) \
    instructions[c].op_ra_rb.op = _op; \
    instructions[c].op_ra_rb.ra = _ra; \
    instructions[c].op_ra_rb.rb = _rb; c++;
#define put_op_ra_rb_imm(_op, _ra, _rb, _imm) \
    instructions[c].op_ra_rb_imm.op = _op; \
    instructions[c].op_ra_rb_imm.ra = _ra; \
    instructions[c].op_ra_rb_imm.rb = _rb; \
    instructions[c].op_ra_rb_imm.imm = _imm; c++;
#define put_op_ra_rb_rc(_op, _ra, _rb, _rc) \
    instructions[c].op_ra_rb_rc.op = _op; \
    instructions[c].op_ra_rb_rc.ra = _ra; \
    instructions[c].op_ra_rb_rc.rb = _rb; \
    instructions[c].op_ra_rb_rc.rc = _rc; c++;
#define ret_instrs() return instructions;

#define reg(n) reg_from(s->params[n])
#define imm(n) imm_from(prog, s->params[n])

#define genOpCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op(op); \
    ret_instrs(); \
}

#define genOpRaCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op_ra(op, reg(0)); \
    ret_instrs(); \
}

#define genOpRaImmCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op_ra_imm(op, reg(0), imm(1)); \
    ret_instrs(); \
}

#define genOpRaRbCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op_ra_rb(op, reg(0), reg(1)); \
    ret_instrs(); \
}

#define genOpRaRbImmCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op_ra_rb_imm(op, reg(0), reg(1), imm(2)); \
    ret_instrs(); \
}

#define genOpRaRbRcCompiler(op, name) \
Instruction* name(StatementList *prog, LineStatement *s) { \
    init_instrs(1); \
    put_op_ra_rb_rc(op, reg(0), reg(1), reg(2)); \
    ret_instrs(); \
}

typedef Instruction* (*InstructionCompiler)(StatementList *prog, LineStatement *s);

typedef struct compilerSpec {
    char *mnemonic;
    InstructionCompiler compile;
    size_t size; // in number of resulting instructions
} CompilerSpec;

extern CompilerSpec instructionCompilers[];

CompilerSpec *getstatementspec(LineStatement *s);

size_t getstatementsize(LineStatement *s);

Instruction *get_instructions(int count, ...);

#endif //REGULARVM_COMPILER_H
