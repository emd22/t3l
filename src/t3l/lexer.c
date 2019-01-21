#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include <t3l/lexer.h>

#define MAX_TOKEN_SIZE 256

// #define TOKEN_PEEK(index)  lexp[index+1]
// #define TOKEN_PEEKP(index) lexp[index-1]
// #define TOKEN_NEXT(index)  lexp[index++]
// #define TOKEN_PREV(index)  lexp[index--]

char **lexp;
unsigned amt_tokens = 0;
unsigned lex_index = 0;

const char operators[] = "+-=*/:$(),";

unsigned set_amt_tokens(char *data, long fsize) {
    bool skipped = false;
    bool cont = false;
    unsigned i;
    int j;
    unsigned tok_len = 0;
    const int op_len = strlen(operators);

    for (i = 0; i < fsize; i++) {
        if (data[i] == '/' && data[i+1] == '/')
            while (data[i++] != '\n');

        if (data[i] == ' ' || data[i] == '\t' || data[i] == '\n' || i == fsize-1) {
            if (skipped)
                continue;
            tok_len = 0;
            skipped = true;
            amt_tokens++;
            continue;
        }

        for (j = 0; j < op_len; j++) {
            if (data[i] == operators[j]) {
                if (tok_len > 0 && !skipped)
                    amt_tokens++;
                amt_tokens++;
                tok_len = 0;
                cont = true;
                skipped = true;
                break;
            }
        }
        if (cont) {
            cont = false;
            continue;
        }
        tok_len++;
        skipped = false;
    }
    return i;
}

lexer_data_t lex(char *data, long fsize) {
    unsigned i;
    set_amt_tokens(data, fsize);
    printf("Token buffer size = %u bytes, %u tokens\n", MAX_TOKEN_SIZE*amt_tokens, amt_tokens);
    lexp = malloc(sizeof(char *)*(amt_tokens));
    for (i = 0; i < amt_tokens; i++) {
        lexp[i] = malloc(MAX_TOKEN_SIZE);
    }

    char ch;
    char this_tok[MAX_TOKEN_SIZE];
    unsigned this_tok_i = 0;
    int j;
    const int op_len = strlen(operators);

    bool in_brackets = false;
    bool skipped = false;
    bool cont = false;
    for (i = 0; i < fsize; i++) {
        if (data[i] == '/' && data[i+1] == '/')
            while (data[i++] != '\n');
        
        ch = data[i];

        if (ch == '{')
            in_brackets = true;
        else if (ch == '}')
            in_brackets = false;

        if (ch == ' ' || ch == '\t' || ch == '\n' || i == fsize-1) {
            if (skipped || in_brackets)
                continue;

            // if on last character in buffer, add it to finish the last token.
            if (i == fsize-1)
                this_tok[this_tok_i++] = data[i];

            skipped = true;
            strcpy(lexp[lex_index++], this_tok);
            memset(this_tok, 0, MAX_TOKEN_SIZE);
            this_tok_i = 0;
            continue;
        }
        for (j = 0; j < op_len; j++) {
            if (data[i] == operators[j]) {
                if (this_tok_i > 0 && !skipped) {
                    memcpy(lexp[lex_index++], this_tok, i);
                }
                lexp[lex_index][0] = data[i];
                lexp[lex_index++][1] = 0;

                memset(this_tok, 0, MAX_TOKEN_SIZE);
                this_tok_i = 0;
                cont = true;
                skipped = true;
                break;
            }
        }
        if (cont) {
            cont = false;
            continue;
        }
        skipped = false;
        this_tok[this_tok_i++] = data[i];
    }

    lexer_data_t lex_data;
    lex_data.lexp = lexp;
    lex_data.amt_tokens = amt_tokens;
    lex_data.token_length = MAX_TOKEN_SIZE;

    return lex_data;
}

void lex_destroy() {
    unsigned i;
    for (i = 0; i < amt_tokens; i++) {
        free(lexp[i]);
    }
    free(lexp);
}