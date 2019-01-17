#include <t3l/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

static int error_count = 0;

int t3l_get_error_count(void) {
    return error_count;
}

void t3l_error(const char *str, ...) {
    va_list args;
    va_start(args, str);

    #ifdef __linux__
    printf("\x1B[91mERROR:\x1B[0;0m ");
    #else
    printf("ERROR: ");
    #endif
    vprintf(str, args);
    va_end(args);
    error_count++;
}

void t3l_lex_error(int col, int row, const char *str, ...) {
    va_list args;
    va_start(args, str);

    #ifdef __linux__
    printf("\x1B[91mERROR:\x1B[0;0m ");
    #else
    printf("ERROR: ");
    #endif
    printf("[%d,%d]: ", col, row);
    vprintf(str, args);
    va_end(args);
    error_count++;
}