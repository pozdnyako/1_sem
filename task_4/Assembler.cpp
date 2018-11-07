#include "StringArray.h"
#include "Assembler.h"

void AssCtor(Assembler *ass) {
    assert(ass);
    assert(!ass->code);
    assert(!ass->bytecode);

    ass->code = new StringArray();

    ass->is_loaded = false;
    ass->is_compeled = false;

    ass->lbls = new Label[MAX_NUM_LABELS];

    ass->n_comp = 0;
}

void AssDtor(Assembler *ass) {
    assert(ass);
    assert(ass->code);

    if(ass->is_compeled)
        assert(ass->bytecode);
}

int AssInput(Assembler *ass, const char* path) {
    assert(ass);
    assert(ass->code);

    ass->code->scan(path);
    ass->code->mark_text();
}

#define WRITE_COMMAND_NAME\
        if(n_words != n_args + 1 && ass->n_comp == 1) {\
            printf("[ERROR] syntaxis error 'wrong argumnets', line %d\n", i+1);\
            return 0;\
        }\
        if(t_bytecode == NORMAL_BYTECODE){\
            memcpy(ass->bytecode + current_pos, (char*)&n_command, sizeof(char));\
            fprintf(log, "%04d %04d ", current_pos, n_command); \
            current_pos += sizeof(char);\
        } else if(t_bytecode == NO_BYTECODE) {\
        } else {\
            memcpy(ass->bytecode + current_pos, (char*)&t_bytecode, sizeof(char));\
            fprintf(log, "%04d %04d ", current_pos, t_bytecode); \
            current_pos += sizeof(char);\
        }

int AssCompile(Assembler *ass) {
    assert(ass);
    assert(ass->code);

     if(ass->n_comp == 0) {
        printf("[OUT]\tcompiling starts\n");
     }

    ass->code->print("out.txt");

    delete[] ass->bytecode;
    ass->bytecode = new char[ass->code->n_strings * MAX_NUM_ARGS * sizeof(double)];

    if(ass->n_comp == 0) {
        for(int i = 0; i < MAX_NUM_LABELS; i ++) {
            ass->lbls[i].position = -1;
            ass->lbls[i].name = "";
        }

        ass->lbl_num = 0;
    }

    int current_pos = 0;

    FILE *log = fopen("log.txt", "wt");

    long long calls[100];
    long long call_n = 0;

    for(int i = 0; i < ass->code->n_strings; i ++) {
        char n_command = -1;
        int n_args = -1;

        char command[MAX_COMMAND_LENGTH];
        strcpy(command, ass->code->strings[i].str);
        if(command[0] == '#')
            continue;

        char words[MAX_NUM_ARGS + 1][MAX_NAME_LENGTH] = {"", "", "", "", ""};
        int n_words = sscanf(command, "%s %s %s %s %s", words[0],
                                                        words[1],
                                                        words[2],
                                                        words[3],
                                                        words[4]);
        if(n_words == -1 || words[0][0] == '#')
            continue;


        if(ass->n_comp == 1) {
            printf("[READ] %10s" "\t" "with %d args : %20s %10s %10s %10s \n", words[0], n_words - 1, words[1],
                                                                                                      words[2],
                                                                                                      words[3],
                                                                                                      words[4]);
        }

        int t_bytecode = 0;
        #define DEF_CMD(name, num, __code, num_arg, name_code, arg_code, _t_bytecode) \
            t_bytecode = _t_bytecode;\
            if(strcmp(words[0], #name_code) == 0 && strlen(words[0]) == strlen(#name_code)){\
                n_command = CMD_##name;\
                n_args = num_arg;\
                \
                WRITE_COMMAND_NAME\
                \
                arg_code\
                \
            }
        #include "commands.h"

        if(ass->n_comp == 1) {
            if(n_command == -1) {
                printf("[ERROR] syntaxis error 'unknow command', line %d\n", i+1);
                return 0;
            }
        }
        #undef DEF_CMD

        fprintf(log, "\n");
    }

    ass->bytecode_size = current_pos;

    if(ass->n_comp == 0) {
        ass->n_comp = 1;
        AssCompile(ass);
    } else {
        printf("[OUT]\tcompiling finished succsessful\n");
        ass->n_comp = 0;

        fclose(log);
    }

    return 1;
}

int AssOutput(Assembler *ass, const char *path) {
    FILE *exe = fopen(path, "wb");

    for(int i = 0; i < ass->bytecode_size; i ++ ){
        fprintf(exe, "%c", ass->bytecode[i]);
    }

    fclose(exe);
}

#undef WRITE_COMMAND_NAME
