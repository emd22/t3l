#ifndef COMPILER_COMPILER_H
#define COMPILER_COMPILER_H

#include <compiler/op/int.h>
#include <compiler/op/mov.h>
#include <compiler/op/func.h>
#include <compiler/op/stack.h>
#include <compiler/registers.h>

void compile(parser_obj_t *pobjs, unsigned amt_pobjs);

#endif