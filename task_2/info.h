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

class StringArray;

bool print_str(int, char*);
bool fprint_str(int, char*, FILE**);
bool scan_str_with_delete_and_new(const char* path, int *buf_size, char* *buf);
