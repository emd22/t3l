#include <t3l/parser.h>
#include <t3l/error.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define OBJS_START 256

#define WORD_MAX (2^16)
#define BYTE_MAX (2^8)

parser_obj_t *objs;
unsigned obj_idx = 0;

void push_obj(int type, int value) {
    parser_obj_t *this_obj = &objs[obj_idx++];
    this_obj->type = type;
    this_obj->value = value;
}

int get_reg_n(char *str) {
    const char *registers[] = {
        "_Null",
        // AX, Lo, Hi
        "_Al",
        "_Ah",
        "_Ax",
        // CX, Lo, Hi
        "_Cl",
        "_Ch",
        "_Cx",
        // DX, Lo, Hi
        "_Dl",
        "_Dh",
        "_Dx",
        // BX, Lo, Hi
        "_Bl",
        "_Bh",
        "_Bx",
    };

    int i;
    int regsz = sizeof(registers)/sizeof(char *);
    for (i = 0; i < regsz; i++) {
        if (!strcmp(str, registers[i])) {
            return i;
        }
    }
    return VR_NULL;
}

parser_obj_t *parse(char **tokens, unsigned tokens_amt, unsigned token_size) {
    objs = malloc(OBJS_START*sizeof(parser_obj_t));
    memset(objs, 0, OBJS_START*sizeof(parser_obj_t));

    char *this_tok;

    unsigned i;
    for (i = 0; i < tokens_amt; i++) {
        this_tok = tokens[i];
        if (this_tok[0] == '#')
            push_obj(V_POINTER, atoi(this_tok+1));
        else if (isdigit(this_tok[0])) {
            int val = atoi(this_tok);
            if (val > WORD_MAX) {
                t3l_error("Value greater than data type");
                push_obj(V_NULL, 0);
            }
            push_obj(V_NUMBER, val);
        }
        else if (this_tok[0] == '_' && (this_tok[1] >= 'A' && this_tok[1] <= 'Z')) {
            push_obj(V_REGISTER, get_reg_n(this_tok));
        }
        else if (!strcmp(this_tok, "_mov"))
    }
    return objs;
}