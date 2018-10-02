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

static const int NO_LETTER = -1;

class StringArray;

struct M_str{
    char* str;
    size_t len;

    M_str()
    :str(NULL)
    ,len(0) {}
};

long long get_file_size(FILE**);

bool print_str(int, char*);
bool fprint_str(M_str, FILE**);

int qsorter_alphabet(const void *a, const void *b);
int qsorter_rhyme(const void *a, const void *b);

bool scan_str_with_delete_and_new(const char* path, size_t *buf_size, char* *buf);
