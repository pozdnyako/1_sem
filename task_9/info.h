#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <cstdlib>

#pragma once

class AST;

class Token;

class Parser;

class Note;
class Tree;


size_t getFileSize(FILE* file);
int readFile(FILE* file, char **buf, size_t* buf_len);
size_t strStartCmp(const char* str1, const char* str2);
bool _strCmp(const char* str1, const int str1_len,
             const char* str2, const int str2_len);

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

