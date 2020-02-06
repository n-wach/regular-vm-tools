//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_TOKEN_H
#define REGULARVM_TOKEN_H

typedef enum token_type {
    OPERATOR,
    VALUE
} TokenType;

typedef enum operator {
    PLUS='+',
    MINUS='-',
    MUL='*',
    DIV='/',
    L_P='(',
    R_P=')',
    LBL='$',
} Operator;

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

void push_operator(TokenStack *stack, Operator op);

void push_value(TokenStack *stack, int value);

void move_top(TokenStack *from, TokenStack *to);

void reverse(TokenStack *stack);

#endif //REGULARVM_TOKEN_H
