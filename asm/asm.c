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
    printstatementl(&p);

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

    printf("Done.\n");
}

int main(int argc, char *argv[]) {
    if(argc != 3) {
        printf("Usage: %s [input] [output]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "r");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    FILE *output = fopen(argv[2], "wb");
    if(output == NULL) {
        printf("Invalid output file");
        fclose(input);
        return -1;
    }

    assemble(input, output);

    fclose(input);
    fclose(output);
}
