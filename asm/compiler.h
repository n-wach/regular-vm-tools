//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_COMPILER_H
#define REGULARVM_COMPILER_H

#include <stddef.h>
#include "../common/instructions.h"

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
