#ifndef PARSER_H
#define PARSER_H

#ifndef TOKEN_END
#define TOKEN_END 0x03
#endif

enum {
    VI_NULL,
    VI_DECL,
    // VI_MOV,
    // VI_JMP,
    // VI_CLI,
    // VI_STI,
    // VI_HLT,
    // VI_INT,
    // VI_FILL,
    // VI_BYTE,
};

enum {
    V_NULL,
    V_NUMBER,
    V_DECL,
    V_REGISTER,
    V_LABEL,
    V_CALL,

    V_STARTBLOCK,
    V_ENDBLOCK,

    V_STARTPARAN,
    V_ENDPARAN,

    V_PARAMETER,
    V_EXPRESSION,
};

typedef struct {
    int value;
    char *label;
    int type;
    int tloc;
} parser_obj_t;

parser_obj_t *parse(char **tokens, unsigned tokens_index, unsigned token_size, int *pobj_amt);

#endif