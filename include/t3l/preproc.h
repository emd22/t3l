#ifndef PREPROC_H
#define PREPROC_H

#define CFLAG_16B 1

void preproc(char **tokens, unsigned *tok_amt, unsigned tok_size);
int get_compiler_flags(void);

#endif