#ifndef COMPILER_FUNC_H
#define COMPILER_FUNC_H

void compiler_call(int offset);
void compiler_ret_val(unsigned short value);
void compiler_ret(void);

#endif