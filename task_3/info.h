/*!
    \file
    \brief Declaration and info file
    \author Yaroslav Pozndyak

    File with all standrad includes and decloration of the global functions
*/
#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>

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

struct Stack;
