//
// Created by me on 2/5/20.
//

#include "compiler.h"
#include "../calc/calc.h"
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
    init_instrs(1)
    put_op_ra_imm(SET, PC, imm(0))
    ret_instrs()
}

Instruction* addiCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(2)
    put_op_ra_imm(SET, AT0, imm(1))
    put_op_ra_rb_rc(ADD, reg(0), reg(0), AT0)
    ret_instrs()
}

Instruction* subiCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(2)
    put_op_ra_imm(SET, AT0, imm(1))
    put_op_ra_rb_rc(SUB, reg(0), reg(0), AT0)
    ret_instrs()
}

Instruction* incCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(2)
    put_op_ra_imm(SET, AT0, 1)
    put_op_ra_rb_rc(ADD, reg(0), reg(0), AT0)
    ret_instrs()
}

Instruction* decCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(2)
    put_op_ra_imm(SET, AT0,  1)
    put_op_ra_rb_rc(SUB, reg(0), reg(0), AT0)
    ret_instrs()
}

Instruction* pushCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(3)
    put_op_ra_rb(STW, SP, reg(0))
    put_op_ra_imm(SET, AT0, 4)
    put_op_ra_rb_rc(SUB, SP, SP, AT0)
    ret_instrs()
}

Instruction* popCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(3)
    put_op_ra_imm(SET, AT0, 4)
    put_op_ra_rb_rc(ADD, SP, SP, AT0)
    put_op_ra_rb(LDW, reg(0), SP)
    ret_instrs()
}

Instruction* jzCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(10)
    put_op_ra_imm(SET, AT0, 0)
    put_op_ra_rb_rc(TCU, AT1, reg(0), AT0)
    put_op_ra_imm(SET, AT0, 4)
    put_op_ra_rb_rc(ADD, AT1, AT1, AT1)
    put_op_ra_rb_rc(ADD, AT1, AT1, AT1)
    put_op_ra_rb_rc(ADD, AT1, AT1, AT0)
    put_op_ra_rb_rc(ADD, PC, PC, AT1)
    put_op_ra_rb_rc(ADD, PC, PC, AT0)
    put_op_ra_imm(SET, PC,imm(1))
    put_op(NOP)
    ret_instrs()
}

Instruction* haltCompiler(StatementList *prog, LineStatement *s) {
    init_instrs(1)
    put_op_ra_imm(SET, PC,s->assembledLocation)
    ret_instrs()
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
        {"ADDI", addiCompiler, 2},
        {"SUBI", subiCompiler, 2},
        {"DEC", decCompiler, 2},
        {"INC", incCompiler, 2},
        {"PUSH", pushCompiler, 3},
        {"POP", popCompiler, 3},
        {"JZ", jzCompiler, 10},
        {"HALT", haltCompiler, 1},
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

int imm_from(StatementList *prog, char *exp) {
    return eval(prog, exp);
}
