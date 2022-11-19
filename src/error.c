#include "error.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

void exit_program(int err_code, char* msg) {
    switch (err_code) {
        case 51:
            fprintf(stderr, "syntax error: %s\n", msg);
            exit(51);
        case 3:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(3);
        case 4:
            fprintf(stderr, "semantic error: %s\n", msg);
            exit(4);
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
