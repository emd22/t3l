#include <t3l/preproc.h>
#include <t3l/lexer.h>
#include <t3l/error.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define NEWLINE   0x0A
#define HORIZ_TAB 0x09
#define BACKSPACE 0x08
#define BELL      0x07

int compiler_flags = 0;

void check_multichar(char *tok, char *value) {
    if (tok[1] == '\\') {
        switch (tok[2]) {
            case 'n':
                (*value) = NEWLINE;
                break;
            case 't':
                (*value) = HORIZ_TAB;
                break;
            case 'b':
                (*value) = BACKSPACE;
                break;
            case 'a':
                (*value) = BELL;
                break;
            default:
                t3l_error("Multi-character constant");
                return;
        }
    }
}

int is_escape(char *tok, unsigned tok_size) {
    int end = strlen(tok);
    char value = 0;
    if (tok[end-1] != '\'') {
        t3l_error("Character object not ended [%s]\n", tok);
        return 1;
    }
    if (end > 3) { /* Multiple characters in character object, might be an escape character */
        check_multichar(tok, &value);
    }
    else {
        value = tok[1];
    }

    char buf[8];
    memset(buf, 0, 8);
    sprintf(buf, "%d", value);
    memset(tok, 0, tok_size);
    memcpy(tok, buf, 8);
    return 0;
}

void insert(char **arr, char *value, unsigned location, unsigned arr_len, unsigned val_len) {

}

void preproc(char **tokens, unsigned *tok_amt, unsigned tok_size) {
    unsigned i;
    static bool done = false;
    char *this_tok;
    for (i = 0; i < (*tok_amt); i++) {
        this_tok = tokens[i];
        if (this_tok[0] == '\'') {
            if (is_escape(this_tok, tok_size))
                break;
        }
        else if (this_tok[0] == '$') {
            if (!strcmp(tokens[i+1], "macro")) {
                unsigned i2;
                for (i2 = 0; i2 < (*tok_amt); i2++) {
                    if (!strcmp(tokens[i2], tokens[i+2]) && strcmp(tokens[i2-1], "macro")) {
                        printf("tokens[%d+2] (%d) = [%s]\n", i, i2, tokens[i+2]);
                        memset(tokens[i2], 0, tok_size);
                        memcpy(tokens[i2], tokens[i+3], tok_size);
                    }
                }
            }
            else if (!strcmp(tokens[i+1], "flag")) {
                printf("Compiler flag %s activated.\n", tokens[i+2]);
                compiler_flags |= atoi(tokens[i+2]);
            }
            else if (!strcmp(tokens[i+1], "include")) {
                char *path = tokens[i+2];
                if (done)
                    continue;
                printf("including file '%s'\n", path);
                FILE *included_fp = fopen(path, "rb");
                if (!included_fp) {
                    t3l_error("Cannot include file '%s' (File does not exist)\n", path);
                    continue;
                }
                fseek(included_fp, 0, SEEK_END);
                unsigned long fsize = ftell(included_fp);
                rewind(included_fp);

                char *fdata = malloc(fsize);
                fread(fdata, 1, fsize, included_fp);

                lexer_data_t old_l_data = get_lexer_data();

                lexer_data_t lexd = lex(fdata, fsize);

                int k;
                for (k = 0; k < lexd.tokens_index; k++) {
                    printf("LXtk: %s\n", lexd.lexp[k]);
                }
                tokens = realloc(tokens, ((*tok_amt)*tok_size)+(lexd.tokens_index*lexd.token_length));
                for (k = 0; k < lexd.tokens_index; k++) {
                    printf("mallocd @ %d\n", (*tok_amt)+k);
                    tokens[k+(*tok_amt)] = malloc(lexd.token_length);
                }


                // unsigned c;
                // for (c = (*tok_amt) - 1; c > i; c--) {
                //     printf("tk[%d] = %s\n", c, tokens[c]);
                //     memcpy(tokens[c+lexd.tokens_index], tokens[c], tok_size);
                // }
                
                for (k = 0; k < (*tok_amt)-i; k++) {
                    memcpy(tokens[i+k+lexd.tokens_index], tokens[i+k], lexd.token_length);
                }

                // return;
                memcpy(tokens[lexd.tokens_index], lexd.lexp, lexd.tokens_index*lexd.token_length);
                (*tok_amt) += lexd.tokens_index;

                free(fdata);
                done = true;

                set_lexer_data(&old_l_data);
                // fclose(included_fp);
            }
        }
    }
}

int get_compiler_flags(void) {
    return compiler_flags;
}