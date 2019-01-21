#include <t3l/compiler.h>
#include <t3l/error.h>

FILE *fp = NULL;

void compiler_out(uint8_t val) {
    if (!fp)
        return;
    fputc(val, fp);
}

void compiler_mov(uint8_t mode, uint16_t v1, uint16_t v2) {
    if (mode == MOV_V8_REG) {
        compiler_out(0xB0+(v2-1));
        compiler_out((uint8_t)v1);
    }
    else if (mode == MOV_V16_REG) {
        compiler_out(0xB0+(v2-1));
        // since x86 is little-endian, we have to put the high
        // 8 bits before the low bits.
        compiler_out((uint8_t)(v1 >> 8));
        compiler_out((uint8_t)(v1));
    }
}

void compile(parser_obj_t *objs, unsigned obj_len) {
    parser_obj_t *this_obj;
    int val;
    int reg;

    // WARNING: spaghet
    int i;

    for (i = 0; i < obj_len; i++) {
        this_obj = &objs[i];
        switch (this_obj->type) {
            case V_INSTRUCTION:
                switch (this_obj->value) {
                    case VI_MOV:
                        reg = objs[i+1].value;
                        val = objs[i+2].value;
                        compiler_mov((val > 256) ? MOV_V16_REG : MOV_V8_REG, val, reg);
                        break;

                    case VI_CLI:
                        compiler_out(0xFA);
                        break;

                    case VI_STI:
                        compiler_out(0xFB);
                        break;
                        
                    case VI_HLT:
                        compiler_out(0xF4);
                        break;
                }
                break;
        }
    }
}

void compiler_init(FILE *_fp) {
    if (_fp == NULL) {
        t3l_error("compiler_init: _fp == NULL\n");
        return;
    }
    fp = _fp;
}