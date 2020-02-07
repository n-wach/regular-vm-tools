#include <assert.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>

#include "token.h"
#include "error.h"
#include "../asm/statement.h"

ExpressionResult parse_to_stack(StatementList *prog, TokenStack *stack, char *exp) {
    ExpressionResult result = {EXP_ERR_NONE};
    bool should_neg_mean_num = true;
    int paren_depth = 0;
    for(int i = 0; ; i++) {
        char c = exp[i];
        if(c == '\0' || c == '\n') {
            break;
        } else if(isdigit(c) || (c == '-' && isdigit(exp[i+1]) && should_neg_mean_num)) {
            char * end;
            size_t value = strtol(&exp[i], &end, 0);
            push_value(stack, value);
            i = end - exp - 1;
            should_neg_mean_num = false;
        } else if(c == EXP_LBL) {
            char *start = &exp[i + 1];
            while (isalnum(exp[i + 1])) {
                i++;
            }
            char end = exp[i + 1];
            exp[i + 1] = '\0';

            LineStatement *line = findLabel(prog, start);
            if(line == NULL) {
                result.error = EXP_ERR_UNKNOWN_LBL;
                return result;
            }
            push_value(stack, line->assembledLocation);

            exp[i + 1] = end;

        } else if(c == EXP_ADD || c == EXP_SUB || c == EXP_MUL || c == EXP_DIV) {
            push_operator(stack, c);
            should_neg_mean_num = true;
        } else if(c == EXP_LP) {
            paren_depth++;
            push_operator(stack, c);
            should_neg_mean_num = true;
        } else if(c == EXP_RP) {
            paren_depth--;
            if(paren_depth < 0) {
                result.error = EXP_ERR_EXTRA_RP;
                return result;
            }
            push_operator(stack, c);
            should_neg_mean_num = false;
        } else if(c == ' ') {
            //skip
        } else {
            // characters can only be operators, numbers or spaces
            result.error = EXP_ERR_INVALID_CHAR;
            result.value = c;
            return result;
        }
    }
    if(paren_depth > 0) {
        result.error = EXP_ERR_EXTRA_LP;
        return result;
    }
    reverse(stack);
    return result;
}

ExpressionResult eval_postfix(TokenStack *stack) {
    ExpressionResult result = {EXP_ERR_NONE};

    TokenStack temp = {NULL};
    TokenNode *v1 = NULL;
    TokenNode *v2 = NULL;
    while(stack->top != NULL) {
        TokenNode *n = pop(stack);
        if(n->type == VALUE) {
            push(&temp, n);
        } else {
            v1 = pop(&temp);
            if(v1 == NULL || v1->type != VALUE) {
                result.error = EXP_ERR_MISSING_VALUE;
                result.value = n->value;
                return result;
            }
            v2 = pop(&temp);
            if(v2 == NULL || v2->type != VALUE) {
                result.error = EXP_ERR_MISSING_VALUE;
                result.value = n->value;
                return result;
            }
            switch(n->value) {
                case EXP_ADD:
                    push_value(&temp, v2->value + v1->value);
                    break;
                case EXP_SUB:
                    push_value(&temp, v2->value - v1->value);
                    break;
                case EXP_MUL:
                    push_value(&temp, v2->value * v1->value);
                    break;
                case EXP_DIV:
                    if(v1->value == 0) {
                        result.error = EXP_ERR_DIVIDE_BY_ZERO;
                        return result;
                    }
                    push_value(&temp, v2->value / v1->value);
                    break;
            }
            free(v1);
            free(v2);
            free(n);
        }
    }
    if(temp.top == NULL) {
        result.error = EXP_ERR_EMPTY_EXPRESSION;
        return result;
    } else {
        TokenNode *r = pop(&temp);
        result.value = r->value;
        free(r);
    }
    if(temp.top != NULL) {
        TokenNode *n;
        result.error = EXP_ERR_EXTRA_VALUE;
        while(temp.top != NULL) {
            n = pop(&temp);
            result.value = n->value;
            free(n);
        }
        return result;
    }
    return result;
}

int precedence(int op) {
    switch(op) {
        case EXP_ADD: return 2;
        case EXP_SUB: return 2;
        case EXP_MUL: return 3;
        case EXP_DIV: return 3;
        default: return 0;
    }
}

ExpressionResult shunting_yard(TokenStack *postfixOut, TokenStack *algebraicInput) {
    ExpressionResult result = {EXP_ERR_NONE};
    TokenStack operatorStack = {NULL};
    while(algebraicInput->top != NULL) {
        TokenNode *token = pop(algebraicInput);
        if(token->type == VALUE) {
            push(postfixOut, token);
        } else if (token->value == EXP_LP){
            // Left paren
            push(&operatorStack, token);
        } else if (token->value == EXP_RP) {
            // Right paren
            while(true) {
                if(operatorStack.top == NULL) {
                    result.error = EXP_ERR_EXTRA_RP;
                    return result;
                }
                TokenNode *op = pop(&operatorStack);
                if(op->value == EXP_LP) {
                    free(op);
                    break;
                };
                push(postfixOut, op);
            }
            free(token);
        } else {
            // all other tokens
            while(operatorStack.top != NULL) {
                TokenNode *op = operatorStack.top;
                if(op->value == EXP_LP) break;
                int stack_p = precedence(op->value);
                int new_p = precedence(token->value);
                if(new_p > stack_p) break;
                move_top(&operatorStack, postfixOut);
            }
            push(&operatorStack, token);
        }
    }
    while(operatorStack.top != NULL) {
        move_top(&operatorStack, postfixOut);
    }
    return result;
}


ExpressionResult calc(StatementList *prog, char *exp) {
    // Turns algebraic string into token stack in same order
    TokenStack input_stack = {NULL};
    ExpressionResult result;
    result = parse_to_stack(prog, &input_stack, exp);
    if(result.error != EXP_ERR_NONE) return result;

    // Use shunting yard to change order to postfix
    TokenStack postfix = {NULL};
    result = shunting_yard(&postfix, &input_stack);
    if(result.error != EXP_ERR_NONE) return result;
    reverse(&postfix);

    // Evaluate postfix
    result = eval_postfix(&postfix);
    return result;
}

int eval(StatementList *prog, char *exp) {
    ExpressionResult r = calc(prog, exp);
    if(r.error == EXP_ERR_NONE) {
        return r.value;
    } else {
        print_error(r);
    }
    return 0;
}


