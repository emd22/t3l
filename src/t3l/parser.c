#include <t3l/parser.h>
#include <t3l/error.h>
#include <t3l/registers.h>

#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>


#define OBJS_START 256

#define WORD_MAX (65535)
#define BYTE_MAX (255)

typedef struct {
    char *label;
    int val;
} instruction_t;

parser_obj_t *objs;
unsigned obj_size = OBJS_START;
unsigned obj_idx = 0;

void push_obj(int type, char *label, int value, int tok_loc) {
    if (obj_idx == obj_size) {
        objs = realloc(objs, obj_size *= 2);
    }

    parser_obj_t *this_obj = &objs[obj_idx++];
    this_obj->type = type;
    this_obj->value = value;
    this_obj->label = label;
    this_obj->tloc = tok_loc;
}

int get_reg_n(char *str) {
    const char *register_names[] = {
        "null",
        // AX, Lo, Hi
        "al",
        "cl",
        "dl",
        "bl",

        "ah",
        "ch",
        "dh",
        "bh",
        
        "ax",
        "cx",
        "dx",
        "bx",

        "sp",
        "bp",
        "si",
        "di"
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

char **get_parameters(char **tokens, unsigned tok_idx, unsigned *params_len) {
    unsigned i = tok_idx;

    //get amount of parameters
    unsigned end_of_params = i;
    int j;
    for (j = 0; tokens[j+i][0] != ')'; j++, end_of_params++) {
        if (tokens[(j+i)-1][0] == ',') {
            end_of_params--;
        }
    }

    char **params = malloc(sizeof(char *)*(end_of_params-tok_idx));

    int len;
    j = 0;

    //TODO: rename i and j

    for (i = 0; i < end_of_params-tok_idx; i++, j++) {
        if (tokens[j+tok_idx][0] == ',') {
            i--;
            continue;
        }

        len = strlen(tokens[j+tok_idx]);
        params[i] = malloc(len);
        strcpy(params[i], tokens[j+tok_idx]);

        // push_obj(V_PARAMETER, params[i], i, j+tok_idx);
    }

    (*params_len) = i;

    return params;
}

// char *get_label(char **tokens, unsigned *i) {
//     unsigned last_i = (*i);

//     while (tokens[last_i--][0] != '(');
//     char *label = tokens[last_i];

//     (*i) = last_i;
//     return label;
// }

int parse_label(char **tokens, unsigned tokens_index, unsigned i) {
    char *label = tokens[i-1];

    unsigned last_i = i;
    while (tokens[i++][0] != ')');
    i--;
    printf("tok: %s, %s\n", label, tokens[i]);

    // whoops, looks like this isn't a function, lets return
    if (label[0] == '(') {
        printf("no func here\n");
        return 1;
    }
    
    unsigned params_len;
    // add 2 to last_i to get past the label and start parantheses
    char **params = get_parameters(tokens, last_i+1, &params_len);
    int k;
    for (k = 0; k < params_len; k++) {
        printf("param: %s\n", params[k]);
    }
    
    // declare label
    if (i < tokens_index-1 && tokens[i+1][0] == '{') {
        // declare label
        // if (!strcmp(label, "_start")) {
        //     push_obj(V_ENTRYPOINT,  last_i, i);
        //     return 1;
        // }
        printf("DECLARE %s\n", label);
        push_obj(V_LABEL, label, last_i, i);
        return 0;
    }

    // call label (jmp)
    int j;
    for (j = 0; j < tokens_index; j++) {
        // find the label in the file
        if (!strcmp(label, tokens[j])) {
            while (tokens[j+k+1][0] != ')')
                k++;
            if (tokens[j+k+2][0] == '{')
                break;
        }
    }
    push_obj(V_CALL, label, j-i, i);
    printf("----done----\n");
    return 0;
}

// int parse_label(char **tokens, unsigned tokens_index, unsigned i) {
//     int last_i = i;
//     while (tokens[last_i--][0] != '(');
//     char *label = tokens[last_i];

    
//     if (i < tokens_index-1 && tokens[i+1][0] == ':') {
//         push_obj(V_LABEL, last_i, i);
//         return 1;
//     }

//     // call label (jmp)
//     int j;
//     for (j = 0; j < tokens_index; j++) {
//         if (!strcmp(label, tokens[j]) && tokens[j+3][0] == ':')
//             break;
//     }


//     push_obj(V_CALL, j, i);
//     return 0;
// }

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
        push_obj(V_NULL, NULL, 0, i);
    }

    push_obj(V_NUMBER, NULL, val, i);
    return 0;
}

parser_obj_t *parse(char **tokens, unsigned tokens_index, unsigned token_size, int *pobj_amt) {
    objs = malloc(OBJS_START*sizeof(parser_obj_t));
    memset(objs, 0, OBJS_START*sizeof(parser_obj_t));

    char *this_tok;


    // instruction_t instructions[] = {
    //     {"decl", VI_DECL}
    //     // {"_mov", VI_MOV},
    //     // {"_cli", VI_CLI},
    //     // {"_hlt", VI_HLT},
    //     // {"_int", VI_INT},
    //     // {"_fill", VI_FILL},
    //     // {"_byte", VI_BYTE}
    // };

    unsigned i;//, j;

    // instruction_t *this_instruction;

    for (i = 0; i < tokens_index; i++) {
        this_tok = tokens[i];
        // printf("tk: %s\n", this_tok);

        if (isdigit(this_tok[0])) {
            if (parse_number(tokens, this_tok, i)) {
                continue;
            }
        }

        // if (isalpha(this_tok[0]) || this_tok[0] == '_') {
        //     push_obj(V_EXPRESSION, this_tok, 0, i);
        //     continue;
        // }
        
        // else if (this_tok[0] == '_' && (this_tok[1] >= 'A' && this_tok[1] <= 'Z')) {
        //     // Push register number so the compiler has an easier time
        //     push_obj(V_REGISTER, get_reg_n(this_tok), i);
        // }

        switch(this_tok[0]) {
            case '{':
                push_obj(V_STARTBLOCK, NULL, 0, i);
                break;
            case '}':
                push_obj(V_ENDBLOCK, NULL, 0, i);
                break;
            case '(':
                if (isalpha(tokens[i-1][0]) || tokens[i-1][0] == '_')
                    parse_label(tokens, tokens_index, i);
                push_obj(V_STARTPARAN, NULL, 0, i);
                break;
            case ')':
                push_obj(V_ENDPARAN, NULL, 0, i);
                break;
        }

        // for (j = 0; j < sizeof(instructions)/sizeof(instruction_t); j++) {
        //     this_instruction = &instructions[j];
        //     if (!strcmp(this_tok, this_instruction->label)) {
        //         push_obj(V_INSTRUCTION, this_instruction->val, i);
        //     }
        // }
    }

    // const char *pobj_types[] = {"Null", "Number", "Decl", "Register", "Label", "Call", "StartBlock", "EndBlock", "StartParan", "EndParan", "Parameter"};

    // parser_obj_t *this_pobj;
    
    // for (i = 0; i < obj_idx; i++) {
    //     this_pobj = &(objs[i]);
    //     printf("pobj: %s, %d\n", pobj_types[this_pobj->type], this_pobj->value);
    //     if (this_pobj->label != NULL) {
    //         printf("\tlabel: %s\n", this_pobj->label);
    //     }
    // }
    // printf("fut\n");
    // return objs;
    (*pobj_amt) = obj_idx;
    return objs;
}