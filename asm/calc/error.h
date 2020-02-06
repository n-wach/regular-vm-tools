//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_ERROR_H
#define REGULARVM_ERROR_H

typedef enum error_type {
    NONE,
    INVALID_CHAR,
    EXTRA_R_P,
    EXTRA_L_P,
    MISSING_VALUE,
    EXTRA_VALUE,
    DIVIDE_BY_ZERO,
    EMPTY_EXPRESSION,
} Error;

typedef struct result {
    Error error;
    int value;
} Result;

void print_error(Result res);

#endif //REGULARVM_ERROR_H
