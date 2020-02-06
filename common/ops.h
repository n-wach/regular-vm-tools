//
// Created by me on 1/25/20.
//

#ifndef REGULAR_VM_TOOLS_OPS_H
#define REGULAR_VM_TOOLS_OPS_H

#define MIN_OP 0x0
#define MAX_OP 0x10

typedef enum __attribute__((__packed__)) op {
    NOP=0x00,
    ADD=0x01,
    SUB=0x02,
    AND=0x03,
    ORR=0x04,
    XOR=0x05,
    NOT=0x06,
    LSH=0x07,
    ASH=0x08,
    TCU=0x09,
    TCS=0x0a,
    SET=0x0b,
    MOV=0x0c,
    LDW=0x0d,
    STW=0x0e,
    LDB=0x0f,
    STB=0x10,
    UNKNOWN_OP=0xff,
} Op;


#endif //REGULAR_VM_TOOLS_OPS_H
