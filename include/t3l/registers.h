#ifndef REGISTERS_H
#define REGISTERS_H

enum {
    R_NULL,

    // low 8 bits
    R_AL,
    R_CL,
    R_DL,
    R_BL,

    // high 8 bits
    R_AH,
    R_CH,
    R_DH,
    R_BH,
    
    // all 16 bits
    R_AX,
    R_CX,
    R_DX,
    R_BX
};

#endif