#ifndef LEXER_H
#define LEXER_H

typedef struct {
    char **lexp;
    unsigned amt_tokens;
    unsigned token_length;
} lexer_data_t;

lexer_data_t lex(char *data, long fsize);
void lex_destroy();

#endif