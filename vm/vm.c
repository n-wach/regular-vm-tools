//
// Created by me on 1/25/20.
//


#include <stdio.h>
#include <malloc.h>
#include "vm.h"

VMState state;
u_int8_t *memory;
#define MEM_SIZE 0x100000

int run_instr(const u_int8_t instr[4]) {
    Op op = (Op) instr[0];
    Reg ra = UNKNOWN_REG;
    Reg rb = UNKNOWN_REG;
    Reg rc = UNKNOWN_REG;
    int imm = 0;
    u_int32_t tmp = 0;

    if(op < MIN_OP || op > MAX_OP) {
        debugln("Unknown OP");
        return -1;
    } else {
        InstructionType type = type_of(op);
        switch(type) {
            case OP:
                break;
            case OP_RA:
                ra = instr[1];
                break;
            case OP_RA_IMM:
                ra = instr[1];
                imm = (instr[2] << 8) | instr[3];
                break;
            case OP_RA_RB:
                ra = instr[1];
                rb = instr[2];
                break;
            case OP_RA_RB_IMM:
                ra = instr[1];
                rb = instr[2];
                imm = instr[3];
                break;
            case OP_RA_RB_RC:
                ra = instr[1];
                rb = instr[2];
                rc = instr[3];
                break;
            default:
                break;
        }
        switch(op) {
            case NOP:
                // nothing
                break;
            case ADD:
                state.reg[ra] = state.reg[rb] + state.reg[rc];
                break;
            case SUB:
                state.reg[ra] = state.reg[rb] - state.reg[rc];
                break;
            case AND:
                state.reg[ra] = state.reg[rb] & state.reg[rc];
                break;
            case ORR:
                state.reg[ra] = state.reg[rb] | state.reg[rc];
                break;
            case XOR:
                state.reg[ra] = state.reg[rb] ^ state.reg[rc];
                break;
            case NOT:
                state.reg[ra] = ~state.reg[rb];
                break;
            case LSH:
                // not supported yet
                break;
            case ASH:
                // not supported yet
                break;
            case TCU:
                state.reg[ra] = sign(state.reg[rb] - state.reg[rc]);
                break;
            case TCS:
                // not supported yet
                break;
            case SET:
                state.reg[ra] = imm;
                break;
            case MOV:
                state.reg[ra] = state.reg[rb];
                break;
            case LDW:
                tmp = state.reg[rb];
                state.reg[ra] = memory[tmp] | memory[tmp + 1] << 8 | memory[tmp + 2] << 16 | memory[tmp + 3] << 24;
                break;
            case STW:
                tmp = state.reg[rb];
                memory[state.reg[ra]] = tmp & 0b11111111;
                memory[state.reg[ra + 1]] = (tmp & (0b11111111 << 8)) >> 8;
                memory[state.reg[ra + 2]] = (tmp & (0b11111111 << 16)) >> 16;
                memory[state.reg[ra + 3]] = (tmp & (0b11111111 << 24)) >> 24;
                break;
            case LDB:
                state.reg[ra] &= 0b11111111111111111111111100000000;
                state.reg[ra] |= memory[state.reg[rb]];
                break;
            case STB:
                memory[state.reg[rb]] = state.reg[ra] & 0b11111111;
                break;
            case UNKNOWN_OP:
                break;
            default:
                break;
        }
    }

    return 1;
}

void init_vm() {
    for(int i = 0; i < MAX_REG; i++) {
        state.reg[i] = 0;
    }
}

void print_vm() {
    printf("VM STATE:\n");
    for(int i = 0; i <= MAX_REG; i++) {
        printf("%s: 0x%x \t", reg_name(i), state.reg[i]);
        if(i % 4 == 3) {
            printf("\n");
        }
    }
    printf("Next OP: %s\n", op_name(memory[state.reg[0]]));
    printf("Press Enter to Run Next Instruction");
    getchar();
}

void step_vm() {
    char instr[4];
    u_int32_t pc = state.reg[0];
    instr[0] = memory[pc];
    instr[1] = memory[pc + 1];
    instr[2] = memory[pc + 2];
    instr[3] = memory[pc + 3];
    state.reg[0] += 4;
    run_instr(instr);
}

void run_vm() {
    while(1) {
        print_vm();
        step_vm();
    }
}

int main(int argc, char *argv[]) {
    if(argc != 2) {
        printf("Usage: %s [bin]\n", argv[0]);
        return -1;
    }
    FILE *input = fopen(argv[1], "rb");
    if(input == NULL) {
        printf("Invalid input file");
        return -1;
    }

    printf("Loading...\n");
    memory = malloc(MEM_SIZE);
    if(memory == NULL) {
        printf("Failed to malloc memory\n");
        return -1;
    }

    fseek (input , 0 , SEEK_END);
    size_t fileSize = ftell(input);
    rewind(input);

    if(fileSize > MEM_SIZE) {
        printf("Program too large for memory\n");
        return -1;
    }

    size_t result = fread(memory, 1, fileSize, input);
    if (result != fileSize) {
        printf("Failed to read program into memory\n");
        return -1;
    }

    init_vm();
    run_vm();

    printf("Done.\n");

    free(memory);
    fclose(input);
}
