//
// Created by me on 2/5/20.
//

#include "compiler.h"
#include <string.h>


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


Instruction* pushCompiler(StatementList *prog, LineStatement *s) {
    Instruction *instrs = get_instructions(3, (int) STW, (int) SET, (int) ADD);

    // stw sp ra
    instrs[0].op_ra_rb.ra = SP;
    instrs[0].op_ra_rb.rb = reg_from(s->params[0]);

    // set at0 8
    instrs[1].op_ra_imm.ra = AT0;
    instrs[1].op_ra_imm.imm = 4;

    // add sp sp at0
    instrs[2].op_ra_rb_rc.ra = SP;
    instrs[2].op_ra_rb_rc.rb = SP;
    instrs[2].op_ra_rb_rc.rc = AT0;

    return instrs;
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
        {"PUSH", pushCompiler, 3},
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