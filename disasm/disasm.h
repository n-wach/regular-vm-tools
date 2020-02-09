//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_DISASM_H
#define REGULARVM_DISASM_H

#include "../common/instructions.h"

void disassembleFile(FILE *input, FILE *output);

int decode_instruction(Instruction i, FILE *out);

#endif //REGULARVM_DISASM_H
