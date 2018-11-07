#include "info.h"

struct Label {
    char* name;
    long long position;
};

struct Assembler {
    StringArray *code;
    char *bytecode;
    long long bytecode_size;
    Label *lbls;
    int lbl_num;

    bool is_loaded, is_compeled;

    int n_comp;
};

void AssCtor(Assembler*);
void AssDtor(Assembler*);

int AssInput(Assembler*, const char*);
int AssCompile(Assembler*);

int AssOutput(Assembler*, const char*);
