#include "StringArray.h"
#include "Disassembler.h"

void DasCtor(Disassembler *das) {
    assert(das);
    assert(!das->bytecode);
    assert(!das->code);

    das->is_loaded = false;
    das->is_decompiled = false;
}

void DasDtor(Disassembler *das) {
    assert(das);

    if(das->is_loaded)
        assert(das->bytecode);

    if(das->is_decompiled)
        assert(das->code);

    delete[] das->code;
    delete[] das->bytecode;
}

int DasInput(Disassembler *das, const char* path) {
    assert(das);

    size_t buf_size;
    scan_str_with_delete_and_new(path, &buf_size, &das->bytecode);
    das->bytecode_size = buf_size - 1;

    return 1;
}

int DasDecompile(Disassembler *das) {
    assert(das);

    das->code = new char[MAX_CODE_SIZE];
    long long cur_code_pos = 0;

    printf("[OUT]\tdecompiling starts\n");

    #define DEF_CMD(name, num, proc_code, arg_n, command_name, ass_code,  bytecode_type, das_code, ...)\
        case CMD_##name :\
            cur_code_pos += sprintf(das->code + cur_code_pos, "%s", #command_name);\
            das_code\
            cur_code_pos += sprintf(das->code + cur_code_pos, "\n");\
            break;

    #define proc das
    while(das->cp < das->bytecode_size) {
        switch(das->bytecode[das->cp]) {
            #include "commands.h"
        default:
            printf("[ERORR] function %hd in position %d wasn't declared", das->bytecode[das->cp], das->cp);
            break;
        }
    }

    das->code_size = cur_code_pos;

    printf("[OUT]\tdecompiling finished succsessful\n");
    #undef DEF_CMD
    #undef proc
}

int DasOutput(Disassembler *das, const char *path) {
    assert(path != NULL);

    FILE *output = fopen(path, "wt");

    assert(output);

    fwrite(das->code, sizeof(char), das->code_size, output);

    fclose(output);
}
