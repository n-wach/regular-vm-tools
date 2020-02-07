//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_ERROR_H
#define REGULARVM_ERROR_H

typedef enum expressionError {
    EXP_ERR_NONE,
    EXP_ERR_INVALID_CHAR,
    EXP_ERR_EXTRA_RP,
    EXP_ERR_EXTRA_LP,
    EXP_ERR_MISSING_VALUE,
    EXP_ERR_EXTRA_VALUE,
    EXP_ERR_DIVIDE_BY_ZERO,
    EXP_ERR_EMPTY_EXPRESSION,
    EXP_ERR_UNKNOWN_LBL,
} ExpressionError;

typedef struct expressionResult {
    ExpressionError error;
    int value;
} ExpressionResult;

void print_error(ExpressionResult res);

#endif //REGULARVM_ERROR_H
