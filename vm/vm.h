//
// Created by me on 1/25/20.
//

#ifndef REGULARVM_VM_H
#define REGULARVM_VM_H

#include <stdbool.h>
#include "../common/regs.h"
#include "../common/ops.h"
#include "../common/util.h"
#include "../common/instructions.h"

#define MEM_SIZE 0x10000
#define STACK_BASE MEM_SIZE - 0x100

typedef struct vm {
    uint32_t reg[MAX_REG + 1];
    uint8_t *memory;
    bool halted;
} VM;

uint32_t vmRegR(VM *vm, Reg reg);
void vmRegW(VM *vm, Reg reg, uint32_t value);

uint8_t vmMemR(VM *vm, uint32_t address);
uint8_t *vmMemRP(VM *vm, uint32_t address);
void vmMemW(VM *vm, uint32_t address, uint8_t value);

void vmInit(VM *vm);
void vmRun(VM *vm);
void vmStep(VM *vm);
Instruction vmCurInstr(VM *vm);
Instruction vmRelInstr(VM *vm, int offset);
void vmExecInstr(VM *vm, Instruction instr);
void vmPrint(VM *vm);
void vmDebug(VM *vm);
void vmFree(VM *vm);

void execFile(FILE *input);

#endif //REGULARVM_VM_H
