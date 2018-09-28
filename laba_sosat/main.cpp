#include <cmath>
#include <cassert>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <algorithm>
#include <ctime>
#include <cstdlib>

long long get_file_size(FILE* *file) {
    assert(file != NULL);

    fpos_t cur_pos;
    fgetpos(*file, &cur_pos);

    fseek(*file, 0, SEEK_END);
    long long size = ftell(*file);

    fseek(*file, 0, cur_pos);

    return size;
}

bool scan_str_with_delete_and_new(const char* path, int *buf_size, char* *buf) {
    assert(path != NULL);
    assert(buf_size != NULL);
    assert(buf != NULL);

    FILE* input = fopen(path, "rb");
    assert(input != NULL);

    int size = get_file_size(&input);
    int length = size / sizeof(char);

    *buf_size = 0;
    free(*buf);

    *buf_size = length + 1;
    *buf = (char*)calloc(length + 1, sizeof(char));
    int result = fread(*buf, sizeof(char), size, input);
    *(*buf + length) = EOF;

    if(result != size ) {
        printf("[ERROR] reading from %s error", path);
        return false;
    }

    fclose(input);

    return true;
}

bool print_str(int str_size, char *str) {
    assert(str != NULL);

    for(int i = 0; i < str_size; i ++) {
        printf("%c", str[i]);
    }
    return true;
}

bool fprint_str(int str_size, char *str, FILE* *file) {
    assert(str != NULL);

    for(int i = 0; i < str_size; i ++) {
        fprintf(*file, "%c", str[i]);
    }
    return true;
}

void scan_data(const char* path, int **one, int* one_num, int **pair_1, int **pair_2, int *pair_num) {
    char *str = NULL;
    int str_size = 0;

    scan_str_with_delete_and_new(path, &str_size, &str);

    int n_strings = 0;

    for(int i = 0; i < str_size; i ++) {
        if(str[i] == '\r') {
            str[i] == '\0';
        }
        if(str[i] == '\n' || str[i] == EOF) {
            str[i] = '\0';
            n_strings ++;
        }
    }

    char** strings = (char**)calloc(n_strings, sizeof(char*));
    int* string_size = (int*)calloc(n_strings, sizeof(int));

    int str_counter = 1;
    int str_length = 1;

    free(*one);
    free(*pair_1);
    free(*pair_2);

    strings[0] = str;

    for(int i = 1; i < str_size && str_counter <= n_strings; i ++) {
        if(str[i] == '\0') {
            strings[str_counter] = str + i + 1;
            string_size[str_counter - 1] = str_length;

            str_length = 0;
            str_counter ++;
        } else {
            str_length ++;
        }
    }

    *one_num = 0;
    *pair_num = 0;

    for(int i = 0; i < n_strings; i ++) {
        int num_digits = 0;
        for(int j = 0; j < string_size[i]; j ++) {
            if((j == 0 && isdigit(*(strings[i] + j))) ||
               (j > 0 && isdigit(*(strings[i] + j)) && !isdigit(*(strings[i] + j - 1))) ) {

                num_digits ++;
            }
        }

        if(num_digits == 1) {
            (*one_num) ++;
        } else if(num_digits == 2) {
            (*pair_num) ++;
        } else if(num_digits > 2) {
            printf("uncorrect input\n");
        }
    }

    std::cout << pair_1 << " " << pair_2 << " " << *pair_num << std::endl;
    if(*one_num > 0) {
        *one = (int*)calloc(*one_num, sizeof(int));
    }
    if(*pair_num > 0) {
        *pair_1 = (int*)calloc(*pair_num, sizeof(int));
        *pair_2 = (int*)calloc(*pair_num, sizeof(int));
    }
    std::cout << pair_1 << " " << pair_2 << " " << *pair_num << std::endl;

    int one_count = 0, pair_count = 0;

    for(int i = 0; i < n_strings; i ++) {
        int num_digits = 0;
        int pair[2] = {0, 0};
        for(int j = 0; j < string_size[i]; j ++) {
            std::cout << i << " " << j << std::endl;
            if((j == 0 && isdigit(*(strings[i] + j))) ||
               (j > 0 && isdigit(*(strings[i] + j)) && !isdigit(*(strings[i] + j - 1))) ) {

                num_digits ++;
            }

            if(isdigit(*(strings[i] + j))) {
                pair[num_digits - 1] = pair[num_digits - 1] * 10 + *(strings[i] + j) - '0';
            }
        }

        if(num_digits == 1) {
            *(*one + one_count) = pair[0];
            one_count ++;
        } else if(num_digits == 2) {
            *(*pair_1 + pair_count) = pair[0];
            *(*pair_2 + pair_count) = pair[1];

            pair_count ++;
        }
    }
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");

    char* path_input = "input/1.txt";

    int *one = NULL;
    int *pair_1 = NULL;
    int *pair_2 = NULL;
    std::cout << &(pair_1) << " " << &(pair_2) << std::endl;
    int one_num = 0, pair_num = 0;
    scan_data(path_input, &one, &one_num, &pair_1, &pair_2, &pair_num);

    printf("one: %d\n", one_num);
    for(int i = 0; i < one_num; i ++) {
        printf("%d:\t%d\n", i, one[i]);
    }

    printf("\npair: %d\n", pair_num);
    for(int i = 0; i < pair_num; i ++) {
        printf("%d:\t%d\t%d\n", i, pair_1[i], pair_2[i]);
    }

    return 0;
}
