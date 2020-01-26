//
// Created by me on 1/25/20.
//
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "../common/ops.h"
#include "../common/regs.h"
#include "../common/util.h"

#define put_reg(r, output) r = strtok(NULL, " "); \
                           if(r == NULL) return -1; \
                           putc(reg_from(r), output); \

int parse_instruction(char* line, int len, FILE *output) {
    char * op_s;
    char * ra_s;
    char * rb_s;
    char * rc_s;
    char * imm_s;
    unsigned long imm;

    op_s = strtok (line, " ");
    if(op_s == NULL) {
        return len == 0 ? 1 : -1;
    }
    Op op = op_from(op_s);
    if(op == UNKNOWN_OP) {
        debugln("Unknown OP");
        return -1;
    } else {
        debug(line);
        debug("decoded as:");
        InstructionType type = type_of(op);
        putc(op, output);
        switch(type) {
            case OP:
                debugln("OP");
                putc('\0', output);
                putc('\0', output);
                putc('\0', output);
                break;
            case OP_RA:
                debugln("OP_RA");
                put_reg(ra_s, output);
                putc('\0', output);
                putc('\0', output);
                break;
            case OP_RA_IMM:
                debugln("OP_RA_IMM");
                put_reg(ra_s, output);
                imm_s = strtok(NULL, " ");
                if(imm_s == NULL) return -1;
                imm = strtol(imm_s, NULL, 0);
                putc((imm & 0b1111111100000000) >> 8, output);
                putc(imm & 0b11111111, output);
                break;
            case OP_RA_RB:
                debugln("OP_RA_RB");
                put_reg(ra_s, output);
                put_reg(rb_s, output);
                putc('\0', output);
                break;
            case OP_RA_RB_IMM:
                debugln("OP_RA_RB_IMM");
                put_reg(ra_s, output);
                put_reg(rb_s, output);
                imm_s = strtok(NULL, " ");
                if(imm_s == NULL) return -1;
                unsigned long v = strtol(imm_s, NULL, 0);
                putc(imm & 0b11111111, output);
                break;
            case OP_RA_RB_RC:
                debugln("OP_RA_RB_RC");
                put_reg(ra_s, output);
                put_reg(rb_s, output);
                put_reg(rc_s, output);
                break;
            default:
                debugln("UNKNOWN TYPE");
                return -1;
        }
    }

    return 1;
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

    char *line = NULL;
    size_t len = 0;
    ssize_t nread;

    printf("Reading...\n");
    while ((nread = getline(&line, &len, input)) != -1) {
        // strip newline
        if(nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
            nread -= 1;
        }
        int status = parse_instruction(line, nread, output);
        if(status == -1) {
            printf("Parse error: %s\n", line);

            free(line);
            fclose(input);
            fclose(output);
            return -1;
        }
    }

    printf("Done.\n");

    free(line);
    fclose(input);
    fclose(output);
}
