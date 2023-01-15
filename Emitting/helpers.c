//
// Created by redcube on 15/01/23.
//
#include <gc/gc.h>
#include <string.h>
#include "emitter.h"

char *StringMerge(char *a, char *b) {
    int lenA = strlen(a);
    int lenB = strlen(b);

    char *newBuffer = GC_MALLOC(lenA + lenB + 1);
    memcpy(newBuffer, a, lenA);
    memcpy(&newBuffer[lenA], b, lenB);
    newBuffer[lenA + lenB] = 0;

    return newBuffer;
}

void CodeAppend(Emitter *emt, char *a) {
    emt->Code = StringMerge(emt->Code, a);
}

void Indent(Emitter *emt) {
    for (int i = 0; i < emt->Indentation * TAB_SIZE; i++)
        CodeAppend(emt, " ");
}