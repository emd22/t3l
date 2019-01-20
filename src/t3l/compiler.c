#include <t3l/compiler.h>
#include <t3l/error.h>

#define MOV_VAL_REG 0
#define MOV_REG_REG 1
#define MOV_MEM_REG 2
#define MOV_REG_MEM 3

FILE *fp = NULL;

void compiler_out(uint8_t val) {
    if (!fp)
        return;
    fputc(val, fp);
}

void compiler_mov(uint8_t mode, uint8_t v1, uint8_t v2) {
    if (mode == MOV_VAL_REG) {

    }
}

void compiler_init(FILE *_fp) {
    if (_fp == NULL) {
        t3l_error("compiler_init: _fp == NULL\n");
        return;
    }
    fp = _fp;
}