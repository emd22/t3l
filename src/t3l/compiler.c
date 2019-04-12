#include <t3l/compiler.h>
#include <compiler/compiler.h>
#include <compiler/io.h>

#include <t3l/parser.h>
#include <t3l/error.h>

#include <string.h>

// // #define SHORT_JMP_ADDR(rel_pos) ((rel_pos > 0) ? rel_pos-1 : 0xFF+(rel_pos))

typedef struct {
    char *label;
    unsigned long bin_idx;
} label_t;

label_t labels[512];
int labels_idx = 0;
FILE *fp = NULL;

label_t *find_label(char *label) {
    int i;
    for (i = 0; i < labels_idx; i++) {
        printf("LABEL!, [@%lu]\n", labels[i].bin_idx);

        if (!strcmp(labels[i].label, label))
            return &labels[i];
    }
    t3l_error("Could not find label during call(Label does not exist)\n");
    return NULL;
}


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
    label_t *label;
    unsigned long bidx;

    for (i = 0; i < amt_pobjs; i++) {
        this_pobj = &pobjs[i];
        switch (this_pobj->type) {
            case V_CALL:
                if (!check_builtin_function(this_pobj)) {
                    label = find_label(this_pobj->label);
                    if (label == NULL)
                        break;                    
                    compiler_get_buffer(&bidx);
                    compiler_call(label->bin_idx-bidx);
                }
                break;
            case V_LABEL:
                label = &labels[labels_idx++];
                // get current index in binary
                compiler_get_buffer(&bidx);
                label->bin_idx = bidx;
                label->label = this_pobj->label;
                printf("adding label %s at %lu...\n", label->label, label->bin_idx);
                break;
        }
    }
}