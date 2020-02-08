//
// Created by me on 2/5/20.
//

#include "statement.h"
#include <string.h>
#include <malloc.h>

LineStatement* plstatement(char *line) {
    char *linecpy = strdup(line);

    // first chop off comments
    size_t end = strcspn(linecpy, ";");
    while(linecpy[end - 1] == ' ') end--;
    linecpy[end] = '\0';

    // get rid of leading spaces
    while(linecpy[0] == ' ') {
        linecpy += 1;
    }

    // next we look for a label
    // labels take the form "label:"
    size_t labelEnd = strcspn(linecpy, ":");
    char *label = NULL;
    if(labelEnd != end) {
        // this line has a label
        label = linecpy;
        linecpy[labelEnd] = '\0';
        linecpy = linecpy + labelEnd + 1;
    }

    // now we look for an instruction, followed by a number of comma-separated parameters
    char *instr = strtok(linecpy, " ");

    if(!label && !instr) return NULL;

    LineStatement *s = malloc(sizeof(LineStatement));
    s->label = label;
    s->instr = instr;
    for(int i = 0; i < 4; i++) {
        s->params[i] = strtok(NULL, ",");
        while (s->params[i] != NULL && s->params[i][0] == ' ') {
            s->params[i] += 1;
        }
    }
    s->assembledLocation = 0;
    s->next = NULL;

    return s;
}

StatementList statementlfromfile(FILE *input) {
    StatementList sl = {NULL, NULL};
    char *line = NULL;
    size_t len = 0;
    ssize_t nread;
    while ((nread = getline(&line, &len, input)) != -1) {
        // strip newline
        if(nread > 0 && line[nread - 1] == '\n') {
            line[nread - 1] = '\0';
        }
        // read and add to statementList
        LineStatement *s = plstatement(line);
        if(s) {
            if (sl.head == NULL) sl.head = s;
            if (sl.tail) sl.tail->next = s;
            sl.tail = s;
        }
    }
    return sl;
}

LineStatement *findLabel(StatementList *prog, char *label) {
    LineStatement *cur = prog->head;
    if(label == NULL) return NULL;
    while(cur) {
        if(cur->label) {
            if(strcmp(label, cur->label) == 0) {
                return cur;
            }
        }
        cur = cur->next;
    }
    return NULL;
}

void printstatementl(StatementList *sl) {
    LineStatement *cur = sl->head;
    while(cur) {
        printlstatement(cur);
        cur = cur->next;
    }
}

void printlstatement(LineStatement *s) {
    printf("0x%x:\t", s->assembledLocation);
    if(s->label) {
        printf("%s: ", s->label);
    }
    if(s->instr) {
        printf("%s", s->instr);
    }
    for(int i = 0; s->params[i] && i < 4; i++) {
        if(i != 0) {
            printf(",");
        }
        printf(" %s", s->params[i]);
    }
    printf("\n");
}