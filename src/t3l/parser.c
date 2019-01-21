#include <t3l/parser.h>
#include <t3l/error.h>
#include <t3l/registers.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>


#define OBJS_START 256

#define WORD_MAX (65536)
#define BYTE_MAX (256)

parser_obj_t *objs;
unsigned obj_idx = 0;

void push_obj(int type, int value, int tok_loc) {
    parser_obj_t *this_obj = &objs[obj_idx++];
    this_obj->type = type;
    this_obj->value = value;
    this_obj->tloc = tok_loc;
}

int get_reg_n(char *str) {
    const char *register_names[] = {
        "_Null",
        // AX, Lo, Hi
        "_Al",
        "_Cl",
        "_Dl",
        "_Bl",

        "_Ah",
        "_Ch",
        "_Dh",
        "_Bh",
        
        "_Ax",
        "_Cx",
        "_Dx",
        "_Bx",

        "_Sp",
        "_Bp",
        "_Si",
        "_Di"
    };

    int i;
    int regsz = sizeof(register_names)/sizeof(char *);
    for (i = 0; i < regsz; i++) {
        if (!strcmp(str, register_names[i])) {
            return i;
        }
    }
    return R_NULL;
}

parser_obj_t *parse(char **tokens, unsigned tokens_amt, unsigned token_size, int *pobj_amt) {
    objs = malloc(OBJS_START*sizeof(parser_obj_t));
    memset(objs, 0, OBJS_START*sizeof(parser_obj_t));

    char *this_tok;

    unsigned i;
    for (i = 0; i < tokens_amt; i++) {
        this_tok = tokens[i];
        // printf("tk: %s\n", this_tok);
        if (this_tok[0] == '#')
            push_obj(V_POINTER, atoi(this_tok+1), i);
        else if (isdigit(this_tok[0])) {
            // macros and flags have already been dealt with
            
            if (i > 2 && (tokens[i-2][0] == '$' || tokens[i-3][0] == '$')) {
                continue;
            }

            int val;
            if (this_tok[strlen(this_tok)-1] == 'h') {
                val = strtol(this_tok, NULL, 16);
            }
            else {
                val = atoi(this_tok);
            }

            if (val > WORD_MAX) {
                t3l_error("Value greater than data type, %d > %d\n", val, WORD_MAX);
                push_obj(V_NULL, 0, i);
            }

            push_obj(V_NUMBER, val, i);
        }
        else if (this_tok[0] == '_' && (this_tok[1] >= 'A' && this_tok[1] <= 'Z')) {
            push_obj(V_REGISTER, get_reg_n(this_tok), i);
        }
        else if (this_tok[0] == ')') {
            int last_i = i;
            while (tokens[last_i--][0] != '(');
            char *label = tokens[last_i];
            
            if (i < tokens_amt-1 && tokens[i+1][0] == ':') {
                // declare label
                if (!strcmp(label, "_start")) {
                    push_obj(V_ENTRYPOINT, last_i, i);
                    printf("THY DECLARITH AS ENTRY (%d)\n", last_i);
                    continue;
                }
                push_obj(V_LABEL, last_i, i);
                printf("DECLARE THY\n");
                continue;
            }
            // call label (jmp)
            printf("CALL THY\n");
            int j;
            for (j = 0; j < tokens_amt; j++) {
                if (!strcmp(label, tokens[j]))
                    break;
            }
            push_obj(V_CALL, j, i);
        }
        else if (!strcmp(this_tok, "_mov"))
            push_obj(V_INSTRUCTION, VI_MOV, i);
        else if (!strcmp(this_tok, "_cli"))
            push_obj(V_INSTRUCTION, VI_CLI, i);
        else if (!strcmp(this_tok, "_hlt"))
            push_obj(V_INSTRUCTION, VI_HLT, i);

        printf("tk: %s\n", this_tok);
    }
    (*pobj_amt) = obj_idx;
    return objs;
}