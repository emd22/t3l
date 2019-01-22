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

typedef struct {
    const char *label;
    int val;
} instruction_t;

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

int parse_label(char **tokens, unsigned tokens_amt, unsigned i) {
    int last_i = i;
    while (tokens[last_i--][0] != '(');
    char *label = tokens[last_i];
    
    if (i < tokens_amt-1 && tokens[i+1][0] == ':') {
        // declare label
        if (!strcmp(label, "_start")) {
            push_obj(V_ENTRYPOINT, last_i, i);
            return 1;
        }
        push_obj(V_LABEL, last_i, i);
        return 1;
    }

    // call label (jmp)
    int j;
    for (j = 0; j < tokens_amt; j++) {
        if (!strcmp(label, tokens[j]) && tokens[j+3][0] == ':')
            break;
    }

    push_obj(V_CALL, j, i);
    return 0;
}

int parse_number(char **tokens, char *this_tok, unsigned i) {
    // macros and flags have already been dealt with
    if (i > 2 && (tokens[i-2][0] == '$' || tokens[i-3][0] == '$')) {
        return 1;
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
    return 0;
}

parser_obj_t *parse(char **tokens, unsigned tokens_amt, unsigned token_size, int *pobj_amt) {
    objs = malloc(OBJS_START*sizeof(parser_obj_t));
    memset(objs, 0, OBJS_START*sizeof(parser_obj_t));

    char *this_tok;

    const instruction_t instructions[] = {
        {"_mov", VI_MOV},
        {"_cli", VI_CLI},
        {"_hlt", VI_HLT},
        {"_int", VI_INT},
        {"_fill", VI_FILL},
        {"_byte", VI_BYTE}
    };

    unsigned i, j;

    instruction_t *this_instruction;

    for (i = 0; i < tokens_amt; i++) {
        this_tok = tokens[i];
        // printf("tk: %s\n", this_tok);
        if (this_tok[0] == '#')
            push_obj(V_POINTER, atoi(this_tok+1), i);

        else if (isdigit(this_tok[0])) {
            if (parse_number(tokens, this_tok, i)) {
                continue;
            }
        }
        
        else if (this_tok[0] == '_' && (this_tok[1] >= 'A' && this_tok[1] <= 'Z')) {
            // Push register number so the compiler has an easier time
            push_obj(V_REGISTER, get_reg_n(this_tok), i);
        }

        else if (this_tok[0] == ')') {
            if (parse_label(tokens, tokens_amt, i)) {
                continue;
            }
        }

        for (j = 0; j < sizeof(instructions)/sizeof(instruction_t); j++) {
            this_instruction = &instructions[j];
            if (!strcmp(this_tok, this_instruction->label)) {
                push_obj(V_INSTRUCTION, this_instruction->val, i);
            }
        }
    }
    (*pobj_amt) = obj_idx;
    return objs;
}