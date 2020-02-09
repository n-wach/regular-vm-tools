//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_DISASM_H
#define REGULARVM_DISASM_H

#include "../common/instructions.h"

void decompileFile(FILE *pFile, FILE *pFile1);

int decode_instruction(Instruction i, FILE *out);

#endif //REGULARVM_DISASM_H
