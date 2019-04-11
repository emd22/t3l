#ifndef REGISTERS_H
#define REGISTERS_H
 
enum {
    // Low 8 bits of each general purpose register
    AL,
    CL,
    DL,
    BL,
    // High 8 bits of each general purpose register
    AH,
    CH,
    DH,
    BH
};

enum {
    // 16 bit general purpose registers
    AX,
    CX,
    DX,
    BX,
    // index and pointer registers
    SP,
    BP,
    SI,
    DI
};

#endif