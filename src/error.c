#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exit_program(int err_code, char* msg) {
    switch (err_code) {
        case 1:
            fprintf(stderr, "lexical error: %s\n", msg);
            exit(1);
        case 2:
            fprintf(stderr, "syntax error: %s\n", msg);
            exit(2);
        case 3:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(3);
        case 4:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(4);
        case 5:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(5);
        case 6:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(6);
        case 7:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(7);
        case 8:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(8);
        case 99:
            fprintf(stderr, "internal error: %s\n", msg);
            exit(99);
    }
}

void warning_msg(const char* fmt, ...) {
    va_list args;

    va_start(args, fmt);
    fprintf(stderr, "INVALID_SYNTAX: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
}
void error_exit(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "ERROR: ");
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
    va_end(args);
    exit(1);
}
