//
// Created by me on 1/25/20.
//
#include <stdio.h>
#include <string.h>
#include "statement.h"
#include "../common/instructions.h"
#include "asm.h"

Instruction *get_instructions(int count, ...) {
    va_list ap;
    va_start (ap, count);

    Instruction *instructions = malloc(sizeof(Instruction) * count);

    for (int i = 0; i < count; i++) {
        Instruction b = {
                .op_ra_rb_rc={(Op)(va_arg(ap, int)), 0, 0, 0}
        };
        instructions[i] = b;
    }

    va_end(ap);
    return instructions;
}


genOpCompiler      (NOP,nopCompiler)
genOpRaRbRcCompiler(ADD,addCompiler)
genOpRaRbRcCompiler(SUB,subCompiler)
genOpRaRbRcCompiler(AND,andCompiler)
genOpRaRbRcCompiler(ORR,orrCompiler)
genOpRaRbRcCompiler(XOR,xorCompiler)
genOpRaRbCompiler  (NOT,notCompiler)
genOpRaRbRcCompiler(LSH,lshCompiler)
genOpRaRbRcCompiler(ASH,ashCompiler)
genOpRaRbRcCompiler(TCU,tcuCompiler)
genOpRaRbRcCompiler(TCS,tcsCompiler)
genOpRaImmCompiler (SET,setCompiler)
genOpRaRbCompiler  (MOV,movCompiler)
genOpRaRbCompiler  (LDW,ldwCompiler)
genOpRaRbCompiler  (STW,stwCompiler)
genOpRaRbCompiler  (LDB,ldbCompiler)
genOpRaRbCompiler  (STB,stbCompiler)

Instruction* jmpiCompiler(StatementList *prog, LineStatement *s) {
    Instruction *set = get_instructions(1, (int) SET);
    set->op_ra_imm.ra = PC;
    set->op_ra_imm.imm = imm_from(prog, s->params[0]);
    return set;
}

CompilerSpec instructionCompilers[] = {
        {"NOP", nopCompiler, 1},
        {"ADD", addCompiler, 1},
        {"SUB", subCompiler, 1},
        {"AND", andCompiler, 1},
        {"ORR", orrCompiler, 1},
        {"XOR", xorCompiler, 1},
        {"NOT", notCompiler, 1},
        {"LSH", lshCompiler, 1},
        {"ASH", ashCompiler, 1},
        {"TCU", tcuCompiler, 1},
        {"TCS", tcsCompiler, 1},
        {"SET", setCompiler, 1},
        {"MOV", movCompiler, 1},
        {"LDW", ldwCompiler, 1},
        {"STW", stwCompiler, 1},
        {"LDB", ldbCompiler, 1},
        {"STB", stbCompiler, 1},
        {"JMPI", jmpiCompiler, 1},
};

CompilerSpec *getstatementspec(LineStatement *s) {
    if(s->instr == NULL) return NULL;
    for(size_t i = 0; i < sizeof(instructionCompilers) / sizeof(CompilerSpec); i++) {
        if(strcmp(s->instr, instructionCompilers[i].mnemonic) == 0) {
            return &instructionCompilers[i];
        }
    }
    printf("Unknown instruction %s\n", s->instr);
    return NULL;
}

size_t getstatementsize(LineStatement *s) {
    if(s->instr == NULL) return 0;
    CompilerSpec *spec = getstatementspec(s);
    if(spec) return spec->size;
    return 0;
}

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
