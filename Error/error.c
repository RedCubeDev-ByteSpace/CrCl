//
// Created by redcube on 14/01/23.
//
#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

void Die(char *ErrorMessage, ...) {
    va_list args;
    va_start(args, ErrorMessage);

    printf(KYEL "/!\\ " KRED);
    vfprintf(stdout, ErrorMessage, args);
    printf(KNRM);
    exit(-1);
}

void cprintf(char *color, char *msg, ...) {
    va_list args;
    va_start(args, msg);

    printf("%s", color);
    vfprintf(stdout, msg, args);
    printf("%s", KNRM);
}