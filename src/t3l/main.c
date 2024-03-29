#include <t3l/lexer.h>
#include <t3l/error.h>
#include <t3l/parser.h>
#include <t3l/preproc.h>

#include <t3l/compiler.h>

#include <compiler/compiler.h>
#include <compiler/io.h>

#include <stdio.h>
#include <stdlib.h>

long fsize_get(FILE *fp) {
    fseek(fp, 0, SEEK_END); // seek to end of file
    long fsize = ftell(fp); // get file location (at the end)
    rewind(fp);             // rewind file pointer back to start
    return fsize;
}

int main() {
    const char *fn = "../main.t3l";
    FILE *fp = fopen(fn, "rb");
    FILE *out_fp = fopen("../b.out", "wb");
    if (!fp || !out_fp) {
        // throw error and exit with failure return code
        t3l_error("Cannot open file %s\n", fn);
        goto end;
    }

    long fsize = fsize_get(fp);

    char *fdata = malloc(fsize); // allocate space for file contents
    fread(fdata, 1, fsize, fp);  // read data into buffer
    fclose(fp);

    lexer_data_t ldata;
    ldata = lex(fdata, fsize);
    preproc(ldata.lexp, &(ldata.tokens_index), ldata.token_length);
    
    int i;
    for (i = 0; i < ldata.tokens_index; i++) {
        printf("tk #%d: %s\n", i, ldata.lexp[i]);
    }

    int amt_pobjs;
    parser_obj_t *this_pobj;
    parser_obj_t *pobjs = parse(ldata.lexp, ldata.tokens_index, ldata.token_length, &amt_pobjs);

    free(ldata.lexp);

    const char *pobj_types[] = {"Null", "Number", "Decl", "Register", "Label", "Call", "StartBlock", "EndBlock", "StartParan", "EndParan", "Parameter"};
    
    for (i = 0; i < amt_pobjs; i++) {
        this_pobj = &(pobjs[i]);
        printf("pobj: %s, %d\n", pobj_types[this_pobj->type], this_pobj->value);
        if (this_pobj->label != NULL) {
            printf("\tlabel: %s\n", this_pobj->label);
        }
    }

    compile(pobjs, amt_pobjs);

    unsigned long len = 0;
    unsigned char *compiler_buf = compiler_get_buffer(&len);

    printf("Binary Dump:\n\n");

    unsigned long index = 0;
    for (index = 0; index < len; index++) {
        printf("%02X ", compiler_buf[index]);
    }
    printf("\n");
    // compiler_init(out_fp);
    // compile(pobjs, amt_pobjs);

end:;
    int error_count = t3l_get_error_count();
    if (error_count > 0) {
        #ifdef __linux__
        printf("\x1B[91mFATAL:\x1B[0;0m ");
        #else
        printf("FATAL: ");
        #endif
        printf("Compilation failed with %d error(s).\n", error_count);
        return 1;
    }

    fclose(out_fp);

    free(fdata);
}