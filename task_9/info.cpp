#include "info.h"

size_t getFileSize(FILE* file) {
    assert(file);

    fpos_t cur_pos = 0;
    fgetpos(file, &cur_pos);

    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);

    fseek(file, 0, cur_pos);

    return size;
}

int readFile(FILE* file, char** buf, size_t* buf_len) {
    assert(file);
    assert(buf);
    assert(buf_len);

    size_t fsize = getFileSize(file);
    size_t flen = fsize / sizeof(char);

    *buf_len = flen + 1;
    *buf = (char*)calloc(*buf_len, sizeof(char));

    int result = fread(*buf, sizeof(char), fsize, file);
    *(*buf + *buf_len - 1) = '\0';

    return result > 0 ? 1 : 0;
}

size_t strStartCmp(const char* str1, const char* str2) {
    size_t str2len = strlen(str2);
    for(size_t i = 0; i < str2len; i++) {
        if(str1[i] == '\0')
            return 0;

        if(str1[i] != str2[i])
            return 0;
    }

    return str2len;
}

bool _strCmp(const char* str1, const int str1_len,
             const char* str2, const int str2_len) {

    if(str1_len != str2_len) {
        return false;
    }

    for(int i = 0; i < str1_len; i ++) {
        if(str2[i] != str1[i]) {
            return false;
        }
    }

    return true;
}
