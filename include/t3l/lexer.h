#ifndef LEXER_H
#define LEXER_H

#define TOKEN_END 0x03

typedef struct {
    char **lexp;
    unsigned tokens_index;
    unsigned token_length;
} lexer_data_t;

lexer_data_t lex(char *data, long fsize);
lexer_data_t get_lexer_data(void);
void set_lexer_data(lexer_data_t *lexd);
void lex_destroy();

#endif