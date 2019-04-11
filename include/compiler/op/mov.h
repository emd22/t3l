#ifndef MOV_H
#define MOV_H

enum {
    // register to register
    R8_R8,
    R16_R16,
    // immediate value to register
    I8_R8,
    I16_R16,

    // register to memory
    R8_M8,
    R16_M16,
    // memory to register
    M8_R8,
    M16_R16
};

void compiler_mov(int option, int src, int dest);

#endif