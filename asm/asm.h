//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_ASM_H
#define REGULARVM_ASM_H

#include <string.h>
#include "../common/ops.h"
#include "../common/instructions.h"
#include "statement.h"

typedef Instruction* (*InstructionCompiler)(StatementList *prog, LineStatement *s);

typedef struct compilerSpec {
    char *mnemonic;
    InstructionCompiler compile;
    size_t size; // in number of resulting instructions
} CompilerSpec;


Instruction *get_instructions(int count, ...);


extern CompilerSpec instructionCompilers[];

CompilerSpec *getstatementspec(LineStatement *s);

size_t getstatementsize(LineStatement *s);

#endif //REGULARVM_ASM_H
