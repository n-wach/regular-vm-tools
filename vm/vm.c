//
// Created by me on 1/25/20.
//


#include <stdio.h>
#include <malloc.h>
#include "vm.h"
#include "../common/instructions.h"
#include "../disasm/disasm.h"

uint32_t vmRegR(VM *vm, Reg reg) {
    if(reg > MAX_REG || reg < MIN_REG) {
        printf("Invalid Reg Read: 0x%x\n", reg);
        return 0;
    }
    return vm->reg[reg];
}

void vmRegW(VM *vm, Reg reg, uint32_t value) {
    if(reg > MAX_REG || reg < MIN_REG) {
        printf("Invalid Reg Write: 0x%x\n", reg);
        return;
    }
    if(reg == PC && vm->reg[PC] == value + 4) {
        printf("Loop detected\n");
        vm->halted = true;
    }
    vm->reg[reg] = value;
}

uint8_t vmMemR(VM *vm, uint32_t address) {
    if(address >= MEM_SIZE) {
        printf("Invalid Address Read: 0x%x\n", address);
        return 0;
    }
    return vm->memory[address];
}

uint8_t *vmMemRP(VM *vm, uint32_t address) {
    if(address >= MEM_SIZE) {
        printf("Invalid Address Read: 0x%x\n", address);
        return 0;
    }
    uint8_t *m = vm->memory;
    return m + address;
}

void vmMemW(VM *vm, uint32_t address, uint8_t value) {
    if(address > MEM_SIZE) {
        printf("Invalid Address Write: 0x%x\n", address);
        return;
    }
    vm->memory[address] = value;
}

void vmInit(VM *vm) {
    vm->memory = malloc(MEM_SIZE);
    if(vm->memory == NULL) {
        printf("Unable to allocate memory\n");
    }
    for(int i = 0; i <= MEM_SIZE; i++) {
        vm->memory[i] = 0;
    }
    for(int i = 0; i <= MAX_REG; i++) {
        vm->reg[i] = 0;
    }
    vm->reg[SP] = STACK_BASE;
    vm->halted = false;
}

void vmRun(VM *vm) {
    while(!vm->halted) {
        vmStep(vm);
    }
    vmPrint(vm);
}

void vmStep(VM *vm) {
    Instruction i = vmCurInstr(vm);
    vmRegW(vm, PC, vmRegR(vm, PC) + 4);
    vmExecInstr(vm, i);
}

Instruction vmCurInstr(VM *vm) {
    return vmRelInstr(vm, 0);
}

Instruction vmRelInstr(VM *vm, int offset) {
    Instruction *i = (Instruction *) vmMemRP(vm, vm->reg[PC] + offset * 4);
    if(i == NULL) {
        printf("Tried to read instruction at invalid memory address\n");
        return (Instruction) (uint32_t) 0;
    }
    return *i;
}

void vmExecInstr(VM *vm, Instruction instr) {
    Op op = instr.op_ra_rb_rc.op;
    Reg ra = instr.op_ra_rb_rc.ra;
    Reg rb = instr.op_ra_rb_rc.rb;
    Reg rc = instr.op_ra_rb_rc.rc;
    uint16_t imm0 = instr.op_ra_imm.imm;
    uint16_t imm1 = instr.op_ra_rb_imm.imm;
    uint32_t shift = 0;
    switch(op) {
        case NOP:
            // nothing
            break;
        case ADD:
            vmRegW(vm, ra, vmRegR(vm, rb) + vmRegR(vm, rc));
            break;
        case SUB:
            vmRegW(vm, ra, vmRegR(vm, rb) - vmRegR(vm, rc));
            break;
        case AND:
            vmRegW(vm, ra, vmRegR(vm, rb) & vmRegR(vm, rc));
            break;
        case ORR:
            vmRegW(vm, ra, vmRegR(vm, rb) | vmRegR(vm, rc));
            break;
        case XOR:
            vmRegW(vm, ra, vmRegR(vm, rb) ^ vmRegR(vm, rc));
            break;
        case NOT:
            vmRegW(vm, ra, ~vmRegR(vm, rb));
            break;
        case LSH:
            shift = (int) vmRegR(vm, rc);
            if(shift >= 0) {
                vmRegW(vm, ra, vmRegR(vm, rb) << shift);
            } else {
                vmRegW(vm, ra, vmRegR(vm, rb) >> -shift);
            }
            break;
        case ASH:
            shift = (int) vmRegR(vm, rc);
            if(shift >= 0) {
                vmRegW(vm, ra, (int) vmRegR(vm, rb) << shift);
            } else {
                vmRegW(vm, ra, (int) vmRegR(vm, rb) >> -shift);
            }
            break;
        case TCU:
            vmRegW(vm, ra, sign(vmRegR(vm, rc) - vmRegR(vm, rb)));
            break;
        case TCS:
            vmRegW(vm, ra, sign((int) vmRegR(vm, rc) - (int) vmRegR(vm, rb)));
            break;
        case SET:
            vmRegW(vm, ra, imm0);
            break;
        case MOV:
            vmRegW(vm, ra, vmRegR(vm, rb));
            break;
        case LDW:
            vmRegW(vm, ra,vmMemR(vm, vmRegR(vm, rb) + 0) << 0 |
                    vmMemR(vm, vmRegR(vm, rb) + 1) << 8 |
                    vmMemR(vm, vmRegR(vm, rb) + 2) << 16 |
                    vmMemR(vm, vmRegR(vm, rb) + 3) << 24);
            break;
        case STW:
            vmMemW(vm, vmRegR(vm, ra) + 0, vmRegR(vm, rb) >> 0);
            vmMemW(vm, vmRegR(vm, ra) + 1, vmRegR(vm, rb) >> 8);
            vmMemW(vm, vmRegR(vm, ra) + 2, vmRegR(vm, rb) >> 16);
            vmMemW(vm, vmRegR(vm, ra) + 3, vmRegR(vm, rb) >> 24);
            break;
        case LDB:
            vmRegW(vm, ra,vmMemR(vm, vmRegR(vm, rb)));
            break;
        case STB:
            vmMemW(vm, vmRegR(vm, ra) + 0, vmRegR(vm, rb) >> 0 & 0xff);
            break;
        default:
            printf("Unknown instruction op");
            break;
    }
}

void vmPrint(VM *vm) {
    printf("Registers:\n");
    for(int i = 0; i <= MAX_REG; i++) {
        uint32_t r = vm->reg[i];
        printf(" %3s: %#-4x (% 3d)    ", reg_name(i), r, r);
        if(i % 4 == 3) {
            printf("\n");
        }
    }
    printf("Instructions:\n");
    uint32_t pc = vm->reg[PC];
    for(int i = -2; i <= 2; i++) {
        if(i == 0) {
            printf(" %0#4x >> ", pc);
            decode_instruction(vmCurInstr(vm), stdout);
        } else {
            if(i > 0 || pc > (-i * sizeof(Instruction))) {
                printf(" %0#4lx    ", pc + i * sizeof(Instruction));
                decode_instruction(vmRelInstr(vm, i), stdout);
            }
        }
    }
    printf("Stack:\n");
    uint32_t sp = vm->reg[SP];
    for(int i = 0; i <= 8; i++) {
        if(sp + i * 4 > STACK_BASE) break;
        uint32_t v = *((uint32_t *) vmMemRP(vm, sp + i * 4));
        printf(" %0#4x    %0#8x  (%d)\n", i * 4, v, v);
    }
    printf("Press Enter to Run Next Instruction");
    getchar();
}

void vmFree(VM *vm) {
    free(vm->memory);
}
