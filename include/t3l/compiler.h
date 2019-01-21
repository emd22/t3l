#ifndef COMPILER_H
#define COMPILER_H

#include <t3l/parser.h>

#include <stdio.h>
#include <stdint.h>

#define MOV_V8_REG  0
#define MOV_V16_REG 1
// #define MOV_REG_REG 1
// #define MOV_MEM_REG 2
// #define MOV_REG_MEM 3

void compile(parser_obj_t *objs, unsigned obj_len);
void compiler_init(FILE *_fp);

#endif