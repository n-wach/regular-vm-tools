//
// Created by me on 2/5/20.
//

#ifndef REGULARVM_STATEMENT_H
#define REGULARVM_STATEMENT_H

#include <stdio.h>

typedef struct lineStatement {
    char *label;
    char *instr;
    char *params[4];
    unsigned int assembledLocation;
    struct lineStatement *next;
} LineStatement;

LineStatement *plstatement(char *line);

void printlstatement(LineStatement *s);

typedef struct statementList {
    LineStatement *head;
    LineStatement *tail;
} StatementList;

StatementList statementlfromfile(FILE *input);

LineStatement *findLabel(StatementList *prog, char *label);

void printstatementl(StatementList *sl);

#endif //REGULARVM_STATEMENT_H
