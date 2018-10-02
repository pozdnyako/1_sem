/*!
    \file
    \brief String Array description
    \author Yaroslav Pozndyak

    File with a description of the class StringArray
*/
#include "StringArray.h"

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

    for(int i = 0; i < data_size; i ++) {
        *(data + i) = *(copy->data + i);
    }

    for(int i = 0; i < n_strings; i ++) {
        (*(strings + i)).str = ((*(copy->strings + i)).str - copy->data) + data;
        (*(strings + i)).len = (*(copy->strings + i)).len;
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
    n_strings = 0;

    for(int i = 0; i < data_size; i ++) {
        if(data[i] == '\r' || data[i] == EOF) {
            data[i] = '\0';
        }
        if(data[i] == '\n') {
            data[i] = '\0';
            n_strings ++;
        }
    }

    strings = new M_str[n_strings];

    int str_counter = 1;
    int str_length = 1;

    strings[0].str = data;

    for(int i = 1; i < data_size && str_counter <= n_strings; i ++) {
        if(data[i] == '\0' && !(i + 1 < data_size && data[i + 1]  == '\0')) {
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

/*!
    \brief Quicksorting

    \param  [in]    first                   number of the first element
    \param  [in]    last                    number of the last  element
    \param  [out]   sortrer                 pointer to sorter function

    \return is element number f should be after element number s

*/
bool StringArray :: sort_text(int first, int last, int (*sorter)(const void*, const void*)) {
    if(first >= last)
        return true;

    assert(first >= 0);
    assert(first < n_strings);
    assert(last >= 0);
    assert(last < n_strings);

    qsort(strings + first, (size_t)(last - first), (size_t)sizeof(M_str), sorter);

    return true;
}

/*!
    \brief  generation sonnet

    \param  [in]    quatrains_num               number of the quatrains

    \return sucsess
*/
bool StringArray :: generate_sonnet(int quatrains_num) {
    if(n_strings < DELTA_STRING * 2)
        return false;

    int *string_num = new int[quatrains_num * 4];

    for(int i = 0; i < quatrains_num; i ++) {
        string_num[i * 4 + 0] = rand() % (n_strings - DELTA_STRING * 2) + DELTA_STRING;
        string_num[i * 4 + 2] = string_num[i * 4 + 0] + rand() % (DELTA_STRING * 2) - DELTA_STRING;
        string_num[i * 4 + 1] = rand() % (n_strings - DELTA_STRING * 2) + DELTA_STRING;
        string_num[i * 4 + 3] = string_num[i * 4 + 1] + rand() % (DELTA_STRING * 2) - DELTA_STRING;
    }
    M_str *new_strings = new M_str[quatrains_num * 4];

    for(int i = 0; i < quatrains_num * 4; i ++) {
        new_strings[i] = strings[string_num[i]];
    }

    delete_marking();

    n_strings = quatrains_num * 4;
    strings = new_strings;

    delete[] string_num;

    return true;
}
