#include <t3l/compiler.h>
#include <compiler/compiler.h>
#include <compiler/io.h>

#include <t3l/parser.h>
#include <t3l/error.h>

#include <string.h>

// // #define SHORT_JMP_ADDR(rel_pos) ((rel_pos > 0) ? rel_pos-1 : 0xFF+(rel_pos))

// // typedef struct {
// //     int idx;
// //     int bin_idx;
// // } label_t;

// // label_t labels[256];
// // int labels_idx = 0;
// // FILE *fp = NULL;

// // void compiler_out(uint8_t val) {
// //     if (!fp)
// //         return;
// //     fputc(val, fp);
// // }

// // int find_label(int idx) {
// //     int i;
// //     for (i = 0; i < labels_idx; i++) {
// //         printf("LABEL!, [%d, %d]\n", labels[i].idx, idx);

// //         if (labels[i].idx == idx)
// //             return i;
// //     }
// //     t3l_error("Could not find label during call(Label does not exist)\n");
// //     return -1;
// // }

// // void compiler_mov(uint8_t mode, uint16_t v1, uint16_t v2) {
// //     if (mode == MOV_V8_REG) {
// //         compiler_out(0xB0+(v2-1));
// //         compiler_out((uint8_t)v1);
// //     }
// //     else if (mode == MOV_V16_REG) {
// //         compiler_out(0xB8+(v2-1));
// //         // since x86 is little-endian, we have to put the high
// //         // 8 bits before the low bits.
// //         compiler_out((uint8_t)(v1 >> 8));
// //         compiler_out((uint8_t)(v1));
// //     }
// // }

// void compile(parser_obj_t *objs, unsigned obj_len) {
//     parser_obj_t *this_obj;
//     int val;
//     int reg;

//     // WARNING: spaghet
//     int i, j;
//     int bidx = 0;

//     for (i = 0; i < obj_len; i++) {
//         this_obj = &objs[i];
//         switch (this_obj->type) {
//             case V_INSTRUCTION:
//                 switch (this_obj->value) {
//                     case VI_MOV:
//                     case VI_INT:
//                         bidx += 2;
//                         break;

//                     case VI_FILL:
//                         bidx += (objs[i+1].value);
//                         break;

//                     case VI_CLI:
//                     case VI_STI:
//                     case VI_HLT:
//                         bidx++;
//                         break;
//                 }
//                 break;

//             case V_CALL:
//                 bidx += 2;
//                 break;
//         }
//         if (objs[i].type == V_LABEL || objs[i].type == V_ENTRYPOINT) {
//             label_t *this_label = &labels[labels_idx++];
//             this_label->bin_idx = bidx;
//             this_label->idx = objs[i].value;
//         }
//     }

//     for (i = 0; i < obj_len; i++) {
//         this_obj = &objs[i];
//         switch (this_obj->type) {
//             case V_INSTRUCTION:
//                 switch (this_obj->value) {
//                     case VI_MOV:
//                         reg = objs[i+1].value;
//                         val = objs[i+2].value;
//                         compiler_mov((val > 256) ? MOV_V16_REG : MOV_V8_REG, val, reg);
//                         break;

//                     case VI_CLI:
//                         compiler_out(0xFA);
//                         break;

//                     case VI_STI:
//                         compiler_out(0xFB);
//                         break;
                        
//                     case VI_HLT:
//                         compiler_out(0xF4);
//                         break;
                    
//                     case VI_INT:
//                         compiler_out(0xCD);
//                         compiler_out(objs[i+1].value);
//                         break;

//                     case VI_FILL:
//                         val = ftell(fp);
//                         for (j = 0; j < objs[i+1].value-val; j++) {
//                             compiler_out(0);
//                         }
//                         break;
//                     case VI_BYTE:
//                         compiler_out(objs[i+1].value);
//                         break;
//                 }
//                 break;

//             case V_CALL:
//                 compiler_out(0xEB);
//                 val = find_label(this_obj->value);
//                 if (val == -1)
//                     break;

//                 compiler_out(SHORT_JMP_ADDR(labels[val].bin_idx-ftell(fp)));
//                 break;
//         }
//     }
// }

// // void compiler_init(FILE *_fp) {
// //     if (_fp == NULL) {
// //         t3l_error("compiler_init: _fp == NULL\n");
// //         return;
// //     }
// //     fp = _fp;
// // }

int check_builtin_function(parser_obj_t *pobj) {
    if (!strcmp(pobj->label, "set")) {
        compiler_mov(I8_R8, 5, AL);
        return 1;
    }
    else if (!strcmp(pobj->label, "decl"))
        return 1;
    else if (!strcmp(pobj->label, "stor")) {
        compiler_push_r16(AX);
        return 1;
    }
    return 0;
}

void compile(parser_obj_t *pobjs, unsigned amt_pobjs) {
    unsigned i;
    parser_obj_t *this_pobj;

    for (i = 0; i < amt_pobjs; i++) {
        this_pobj = &pobjs[i];
        switch (this_pobj->type) {
            case V_CALL:
                if (!check_builtin_function(this_pobj)) {
                    compiler_call(this_pobj->value);
                }
                break;
        }
    }
}