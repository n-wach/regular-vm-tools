//
// Created by me on 1/25/20.
//


#include <stdio.h>
#include <malloc.h>
#include "vm.h"
#include "../common/instructions.h"
#include "../disasm/disasm.h"

VMState state;
uint8_t *memory;
#define MEM_SIZE 0x100000

int run_instr(const Instruction instr) {
    Op op = instr.op.op;
    uint32_t tmp = 0;
    switch(op) {
        case NOP:
            // nothing
            break;
        case ADD:
            state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] + state.reg[instr.op_ra_rb_rc.rc];
            break;
        case SUB:
            state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] - state.reg[instr.op_ra_rb_rc.rc];
            break;
        case AND:
            state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] & state.reg[instr.op_ra_rb_rc.rc];
            break;
        case ORR:
            state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] | state.reg[instr.op_ra_rb_rc.rc];
            break;
        case XOR:
            state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] ^ state.reg[instr.op_ra_rb_rc.rc];
            break;
        case NOT:
            state.reg[instr.op_ra_rb.ra] = ~state.reg[instr.op_ra_rb.rb];
            break;
        case LSH:
            tmp = (int) state.reg[instr.op_ra_rb_rc.rc];
            if(tmp >= 0) {
                state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] << tmp;
            } else {
                state.reg[instr.op_ra_rb_rc.ra] = state.reg[instr.op_ra_rb_rc.rb] >> -tmp;
            }
            break;
        case ASH:
            tmp = (int) state.reg[instr.op_ra_rb_rc.rc];
            if(tmp >= 0) {
                state.reg[instr.op_ra_rb_rc.ra] = ((int) state.reg[instr.op_ra_rb_rc.rb]) << tmp;
            } else {
                state.reg[instr.op_ra_rb_rc.ra] = ((int) state.reg[instr.op_ra_rb_rc.rb]) >> -tmp;
            }
            break;
        case TCU:
            state.reg[instr.op_ra_rb_rc.ra] = sign(state.reg[instr.op_ra_rb_rc.rc] - state.reg[instr.op_ra_rb_rc.rc]);
            break;
        case TCS:
            state.reg[instr.op_ra_rb_rc.ra] = sign((int) state.reg[instr.op_ra_rb_rc.rc] - (int) state.reg[instr.op_ra_rb_rc.rc]);
            break;
        case SET:
            state.reg[instr.op_ra_imm.ra] = instr.op_ra_imm.imm;
            break;
        case MOV:
            state.reg[instr.op_ra_rb.ra] = state.reg[instr.op_ra_rb.rb];
            break;
        case LDW:
            state.reg[instr.op_ra_rb.ra] =
                    memory[state.reg[instr.op_ra_rb.ra] + 0] << 0 |
                    memory[state.reg[instr.op_ra_rb.ra] + 1] << 8 |
                    memory[state.reg[instr.op_ra_rb.ra] + 2] << 16 |
                    memory[state.reg[instr.op_ra_rb.ra] + 3] << 24;
            break;
        case STW:
            memory[state.reg[instr.op_ra_rb.ra] + 0] = state.reg[instr.op_ra_rb.rb] >> 0 & 0xff;
            memory[state.reg[instr.op_ra_rb.ra] + 1] = state.reg[instr.op_ra_rb.rb] >> 8 & 0xff;
            memory[state.reg[instr.op_ra_rb.ra] + 2] = state.reg[instr.op_ra_rb.rb] >> 16 & 0xff;
            memory[state.reg[instr.op_ra_rb.ra] + 3] = state.reg[instr.op_ra_rb.rb] >> 24 & 0xff;
            break;
            break;
        case LDB:
            state.reg[instr.op_ra_rb.ra] &= memory[state.reg[instr.op_ra_rb.rb]] & 0xff;
            break;
        case STB:
            memory[state.reg[instr.op_ra_rb.ra]] &= state.reg[instr.op_ra_rb.rb] & 0xff;
            break;
        case UNKNOWN_OP:
            break;
        default:
            break;
    }

    return 1;
}

void init_vm() {
    for(int i = 0; i < MAX_REG; i++) {
        state.reg[i] = 0;
    }
}

void print_vm() {
    printf("Registers:\n");
    for(int i = 0; i <= MAX_REG; i++) {
        printf("%s: 0x%x \t", reg_name(i), state.reg[i]);
        if(i % 4 == 3) {
            printf("\n");
        }
    }
    printf("Instructions:\n");
    for(int i = -2; i < 3; i++) {
        uint32_t pc = state.reg[0];
        if(i == 0) {
            printf(">>");
            decode_instruction(((Instruction *) &memory[pc])[0], stdout);
        } else {
            if(i > 0 || ((int) pc + (i * 4)) >= 0) {
                printf("  ");
                decode_instruction(((Instruction *) &memory[pc + i * 4])[0], stdout);
            }
        }
    }
    printf("Press Enter to Run Next Instruction");
    getchar();
}

void step_vm() {
    Instruction i = ((Instruction *) &memory[state.reg[0]])[0];
    state.reg[0] += 4;
    run_instr(i);
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
