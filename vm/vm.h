//
// Created by me on 1/25/20.
//

#ifndef REGULARVM_VM_H
#define REGULARVM_VM_H

#include <zconf.h>
#include "../common/regs.h"
#include "../common/ops.h"
#include "../common/util.h"

typedef struct vmstate {
    u_int32_t reg[MAX_REG];
} VMState;

#endif //REGULARVM_VM_H
