#ifndef ERROR_C
#define ERROR_C

#include <stdio.h>
#include "error.h"

void print_error(ExpressionResult res) {
    switch(res.error) {
        case EXP_ERR_NONE:
            break;
        case EXP_ERR_INVALID_CHAR:
            printf("Invalid character '%c'\n", res.value);
            break;
        case EXP_ERR_EXTRA_RP:
            printf("Extra ')'\n");
            break;
        case EXP_ERR_EXTRA_LP:
            printf("Unclosed '('\n");
            break;
        case EXP_ERR_MISSING_VALUE:
            printf("ExpressionOperator '%c' does not have two arguments\n", res.value);
            break;
        case EXP_ERR_EXTRA_VALUE:
            printf("Extra value '%d' is missing operator\n", res.value);
            break;
        case EXP_ERR_DIVIDE_BY_ZERO:
            printf("Divide by zero\n");
            break;
        case EXP_ERR_EMPTY_EXPRESSION:
            printf("Empty expression\n");
            break;
        case EXP_ERR_UNKNOWN_LBL:
            printf("Unknown label\n");
            break;
    }
}

#endif