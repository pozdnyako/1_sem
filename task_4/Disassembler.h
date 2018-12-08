#include "info.h"

struct Disassembler {
    char *code;
    char *bytecode;
    long long bytecode_size;
    long long code_size;

    bool is_loaded, is_decompiled;

    long long cp;
};

void DasCtor(Disassembler*);
void DasDtor(Disassembler*);

int DasInput(Disassembler*, const char*);
int DasDecompile(Disassembler*);

int DasOutput(Disassembler*, const char*);

