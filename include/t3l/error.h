#ifndef ERROR_H
#define ERROR_H

#define T3L_NO_RET  -1
#define T3L_FAILURE  1
#define T3L_SUCCESS  0

int t3l_get_error_count(void);
void t3l_error(const char *str, ...);
void t3l_lex_error(int col, int row, const char *str, ...);

#endif