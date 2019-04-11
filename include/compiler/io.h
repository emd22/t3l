#ifndef COMPILER_IO_H
#define COMPILER_IO_H

void compiler_out_one(unsigned char ch);
void compiler_out(unsigned len, ...);
unsigned char *compiler_get_buffer(unsigned long *index);

#endif