#ifndef TOKEN_C
#define TOKEN_C

#include <stdlib.h>
#include "token.h"

TokenNode* pop(TokenStack *stack) {
    TokenNode *top = stack->top;
    if(top == NULL) return NULL;
    stack->top = top->next;
    return top;
}

void push(TokenStack *stack, TokenNode* node) {
    node->next = stack->top;
    stack->top = node;
}

TokenNode* make_node(int value, TokenType type) {
    TokenNode* new_token = malloc(sizeof(TokenNode));
    new_token->type = type;
    new_token->value = value;
    new_token->next = NULL;
    return new_token;
}

void push_operator(TokenStack *stack, ExpressionOperator op) {
    push(stack, make_node(op, OPERATOR));
}

void push_value(TokenStack *stack, int value) {
    push(stack, make_node(value, VALUE));
}

void move_top(TokenStack *from, TokenStack *to) {
    TokenNode *tnode = pop(from);
    push(to, tnode);
}

void reverse(TokenStack *stack) {
    TokenStack rev = {NULL};
    while(stack->top != NULL) {
        push(&rev, pop(stack));
    }
    stack->top = rev.top;
}

#endif