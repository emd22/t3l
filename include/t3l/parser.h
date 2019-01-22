#ifndef PARSER_H
#define PARSER_H

enum {
    VI_NULL,
    VI_MOV,
    VI_JMP,
    VI_CLI,
    VI_STI,
    VI_HLT,
    VI_INT,
    VI_FILL,
    VI_BYTE,
};

enum {
    V_NULL,
    V_NUMBER,
    V_POINTER, // (DWORD)
    V_INSTRUCTION,
    V_REGISTER,
    V_LABEL,
    V_CALL,
    V_ENTRYPOINT,
};

typedef struct {
    int value;
    int type;
    int tloc;
} parser_obj_t;

parser_obj_t *parse(char **tokens, unsigned tokens_amt, unsigned token_size, int *pobj_amt);

#endif