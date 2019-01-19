#include <t3l/lexer.h>
#include <t3l/error.h>
#include <t3l/preproc.h>

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
    if (!fp) {
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
    preproc(ldata.lexp, ldata.amt_tokens, ldata.token_length);
    int i;
    for (i = 0; i < ldata.amt_tokens; i++) {
        printf("token: [%s]\n", ldata.lexp[i]);
    }

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

    free(fdata);
}