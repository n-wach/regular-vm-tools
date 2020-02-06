//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_DISASM_H
#define REGULARVM_DISASM_H

#include <bits/types/FILE.h>
#include "../common/instructions.h"

int decode_instruction(Instruction i, FILE *out);

#endif //REGULARVM_DISASM_H
