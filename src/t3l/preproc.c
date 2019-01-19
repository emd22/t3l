#include <t3l/preproc.h>
#include <t3l/error.h>

#include <stdio.h>
#include <string.h>

#define NEWLINE   0x0A
#define HORIZ_TAB 0x09
#define BACKSPACE 0x08
#define BELL      0x07

// typedef struct {
//     char macro[64];
//     char *start;
//     int size;
// } macro_t;

// macro_t macros[64];
// unsigned marco_index = 0;

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
    if (tok[end] != '\'') {
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

void preproc(char **tokens, unsigned tok_amt, unsigned tok_size) {
    unsigned i;
    char *this_tok;
    for (i = 0; i < tok_amt; i++) {
        this_tok = tokens[i];
        if (this_tok[0] == '\'') {
            if (is_escape(this_tok, tok_size))
                break;
        }
        else if (this_tok[0] == '$') {
            if (!strcmp(tokens[i+1], "macro")) {

            }
        }
    }
}