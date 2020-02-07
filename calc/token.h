//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_TOKEN_H
#define REGULARVM_TOKEN_H

typedef enum token_type {
    OPERATOR,
    VALUE
} TokenType;

typedef enum expressionOperator {
    EXP_ADD='+',
    EXP_SUB='-',
    EXP_MUL='*',
    EXP_DIV='/',
    EXP_LP='(',
    EXP_RP=')',
    EXP_LBL='$',
} ExpressionOperator;

typedef struct token_node {
    TokenType type;
    int value;
    struct token_node *next;
} TokenNode;

typedef struct token_stack {
    TokenNode *top;
} TokenStack;

TokenNode* pop(TokenStack *stack);

void push(TokenStack *stack, TokenNode* node);

TokenNode* make_node(int value, TokenType type);

void push_operator(TokenStack *stack, ExpressionOperator op);

void push_value(TokenStack *stack, int value);

void move_top(TokenStack *from, TokenStack *to);

void reverse(TokenStack *stack);

#endif //REGULARVM_TOKEN_H
