/*!
    \file
    \brief Declaration and info file
    \author Yaroslav Pozndyak

    File with all standrad includes, decloration of the struct Stack and defining DEBUGING
*/
#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#pragma once

struct Proc;
struct Assembler;

struct Stack;
typedef double stack_e;

class StringArray;

struct M_str;

long long get_file_size(FILE**);

bool print_str(int, char*);
bool fprint_str(M_str, FILE**);

int qsorter_alphabet(const void *a, const void *b);
int qsorter_rhyme(const void *a, const void *b);

bool scan_str_with_delete_and_new(const char* path, size_t *buf_size, char* *buf);

const int MAX_COMMAND_LENGTH = 80;
const int MAX_NAME_LENGTH = 30;
const int MIN_NAME_LENGTH = 3;
const int MAX_NUM_ARGS = 4;
const int MAX_NUM_LABELS = 20;

const long long MAX_CODE_SIZE = MAX_COMMAND_LENGTH * 1000;

const int NO_BYTECODE = -2;
const int NORMAL_BYTECODE = -1;

const int position_dont_declared = -1;

enum Comamnds {     // Exmaple 1
    #define DEF_CMD(name, num, ...) CMD_##name = num,
    #include "commands.h"
    CMD_MAX
    #undef DEF_CMD
};


enum Registers {
    #define DEF_REG(name, num) CMD_REG_##name = num,
    #include "registers.h"
    CMD_REG_MAX = 5
    #undef DEF_REG
};



#define DEBUG

#if defined(DEBUG)
    #define PRINTF printf
#else
    #define PRINTF
#endif

#define g_nm(var) #var

#define UNIT_TEST(what, ref) \
    {\
    int _what = (int)what, _ref = (int)ref;\
    if(_what != _ref) \
        PRINTF("[FAILED]\t%s = %d, expected %d\n", #what, _what, _ref); \
    else\
        PRINTF("[PASSED]\t%s = %d\n", #what, _what);\
    }


// for (auto auto: auto) auto();

