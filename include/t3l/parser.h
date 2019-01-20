#ifndef PARSER_H
#define PARSER_H

enum {
    VR_NULL,
    VR_AL,
    VR_AH,
    VR_AX,
    VR_CL,
    VR_CH,
    VR_CX,
    VR_DL,
    VR_DH,
    VR_DX,
    VR_BL,
    VR_BH,
    VR_BX
};

enum {
    V_NULL,
    V_NUMBER,
    V_POINTER, // (DWORD)
    V_INSTRUCTION,
    V_REGISTER,
    V_SEPERATOR,
};

typedef struct {
    int value;
    int type;
} parser_obj_t;

#endif