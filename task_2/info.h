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

class StringArray;

long long get_file_size(FILE**);

bool print_str(int, char*);
bool fprint_str(int, char*, FILE**);

bool scan_str_with_delete_and_new(const char* path, int *buf_size, char* *buf);
