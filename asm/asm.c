//
// Created by me on 1/25/20.
//
#include <stdio.h>
#include <string.h>
#include "statement.h"
#include "../common/instructions.h"
#include "compiler.h"

void assemble(FILE *input, FILE *output) {
    StatementList p = statementlfromfile(input);

    // we need to calculate the offsets now
    LineStatement *cur = p.head;
    size_t offset = 0;
    while(cur) {
        cur->assembledLocation = offset;
        offset += getstatementsize(cur) * sizeof(Instruction);
        cur = cur->next;
    }

    // we can compile and write to file
    cur = p.head;
    while(cur) {
        CompilerSpec *spec = getstatementspec(cur);
        if(spec) {
            Instruction *instructions = spec->compile(&p, cur);
            fwrite(instructions, sizeof(Instruction), spec->size, output);
            free(instructions);
        }
        cur = cur->next;
    }
}
