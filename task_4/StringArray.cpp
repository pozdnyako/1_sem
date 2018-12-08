/*!
    \file
    \brief String Array description
    \author Yaroslav Pozndyak

    File with a description of the class StringArray
*/
#include "StringArray.h"

/*!
    \param  [in]    file            pointer to used file

    \return size of the file in bites
*/
long long get_file_size(FILE* *file) {
    assert(file != NULL);

    fpos_t cur_pos;
    fgetpos(*file, &cur_pos);

    fseek(*file, 0, SEEK_END);
    long long size = ftell(*file);

    fseek(*file, 0, cur_pos);

    return size;
}

/*!
    \brief Input
    \param  [in]    path                pointer to path to input file
    \param  [out]   buf_size            pointer to size of file
    \param  [out]   buf                 pointer to data

    \return sucsess

    \note Delete buf and buf_size. Scan file to buf.
*/
bool scan_str_with_delete_and_new(const char* path, size_t *buf_size, char* *buf) {
    assert(path != NULL);
    assert(buf_size != NULL);
    assert(buf != NULL);

    FILE* input = fopen(path, "rb");
    assert(input != NULL);

    int size = get_file_size(&input) + 1;
    int length = size / sizeof(char);

    *buf_size = 0;
    delete *buf;

    *buf_size = length;
    *buf = new char[length];
    int result = fread(*buf, sizeof(char), size, input);
    *(*buf + length - 1) = EOF;

    if(result != size - 1) {
        printf("[ERROR] reading from %s\n", path);
        return false;
    }

    fclose(input);

    return true;
}

/*!
    \brief Print string
    \param  [in]    str_size                size of string
    \param  [in]    str                     string to printing

    \return sucsess

    \note print string
*/
bool print_str(int str_size, char *str) {
    assert(str != NULL);

    for(int i = 0; i < str_size; i ++) {
        printf("%c", *(str + i));
    }
    return true;
}

/*!
    \brief Print string to file
    \param  [in]    str_size                size of string
    \param  [in]    str                     string to printing
    \param  [in]    file                    pointer to file for printing

    \return sucsess

    \note print string to file
*/
bool fprint_str(M_str str, FILE* *file) {
    assert(str.str != NULL);

    fprintf(*file, "%s", str.str);
    return true;
}

/*!
    \brief Standrad constructor
*/
StringArray :: StringArray()
    :data(NULL)
    ,data_size(0)
    ,strings(NULL)
    ,n_strings(0)
    ,is_marked(false)
    ,is_data_loaded(false){
}

StringArray :: StringArray(StringArray *copy)
    :data(NULL)
    ,data_size(0)
    ,strings(NULL)
    ,n_strings(0)
    ,is_marked(false)
    ,is_data_loaded(false){

    memcopy(copy);
}

bool StringArray :: memcopy(StringArray *copy) {
    delete_marking();
    delete_data();

    assert(copy);

    data_size = copy->data_size;
    n_strings = copy->n_strings;
    is_marked = copy->is_marked;
    is_data_loaded = copy->is_data_loaded;

    data = new char[data_size];
    strings = new M_str[n_strings];

    for(int i = 0; i < (int)data_size; i ++) {
        *(data + i) = *(copy->data + i);
    }

    for(int i = 0; i < (int)n_strings; i ++) {
        (*(strings + i)).str = ((*(copy->strings + i)).str - copy->data) + data;
        (*(strings + i)).len = ( *(copy->strings + i)).len;
    }
}

/*!
    \brief Standard destructor
*/
StringArray :: ~StringArray() {
    delete_marking();
    delete_data();
}

/*!
    \brief Memory cleaner
    \note delete info about strings
*/
bool StringArray :: delete_marking() {
    if(!is_marked)
        return false;

    for(int i = 0; i < n_strings; i ++) {
        strings[i].str = NULL;
    }

    if(strings)
        delete[] strings;
    strings = NULL;

    is_marked = false;

    return true;
}

/*!
    \brief Memory cleaner
    \note delete data
*/
bool StringArray :: delete_data() {
    if(!is_data_loaded)
        return false;

    assert(!is_marked);

    if(data)
        delete[] data;
    data = NULL;

    is_data_loaded = false;

    return true;
}

/*!
    \brief Memory cleaner
*/
bool StringArray :: clean_mem() {
    delete_marking();
    delete_data();
}

/*!
    \brief Scan data
    \param  [in]    path                    path to file

    \return sucsess
*/
bool StringArray :: scan(const char* path) {
    assert(path != NULL);

    is_data_loaded = scan_str_with_delete_and_new(path, &data_size, &data);
    data[data_size - 1] = EOF;

    return is_data_loaded;
}

/*!
    \brief Print data
    \param  [in]    path                    path to file

    \return sucsess
*/
bool StringArray :: print(const char *path) {
    assert(path != NULL);

    FILE *output = fopen(path, "wt");

    assert(output != NULL);

    for(int i = 0; i < n_strings; i ++) {
        fprint_str(strings[i], &output);
        fprintf(output, "\n");
    }

    fclose(output);

    return true;
}

/*!
    \brief Mark data

    \return sucsess

    \note This function divides data to strings and changing '\n' and EOF to '\0', makes array of pointers to begins of strings and array of size of strings
*/
bool StringArray :: mark_text() {
    delete_marking();
    n_strings = 1;

    for(int i = 0; i < data_size; i ++) {
        if(data[i] == '\r' || data[i] == EOF) {
            data[i] = '\0';
        }
        if(data[i] == '\n') {
            data[i] = '\0';
        }
    }
    for(int i = 1; i < data_size; i ++) {
        if(data[i] == '\0' && !(i + 1 < data_size && data[i + 1]  == '\0') || i + 1 == data_size){
            n_strings ++;
        }
    }

    strings = new M_str[n_strings];

    int str_counter = 1;
    int str_length = 1;

    strings[0].str = data;

    for(int i = 1; i < data_size && str_counter <= n_strings; i ++) {
        if(data[i] == '\0' && !(i + 1 < data_size && data[i + 1]  == '\0') || i + 1 == data_size) {
            strings[str_counter].str = data + i + 1;
            strings[str_counter - 1].len = str_length;

            str_length = 0;
            str_counter ++;
        } else {
            str_length ++;
        }
    }

    is_marked = true;
    return true;
}
