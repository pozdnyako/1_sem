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
    ,text(NULL)
    ,string_size(NULL)
    ,n_strings(0)
    ,is_marked(false)
    ,is_data_loaded(false){
}

StringArray :: StringArray(StringArray *copy)
    :data(NULL)
    ,data_size(copy->data_size)
    ,text(NULL)
    ,string_size(NULL)
    ,n_strings(copy->n_strings)
    ,is_marked(copy->is_marked)
    ,is_data_loaded(copy->is_data_loaded) {

    data_size = 0 + data_size;
    n_strings = 0 + n_strings;

    data = new char[data_size];
    text = new char*[n_strings];
    string_size = new int[n_strings];

    for(int i = 0; i < data_size; i ++) {
        *(data + i) = *(copy->data + i);
    }

    for(int i = 0; i < n_strings; i ++) {
        text[i] = (copy->text[i] - copy->data) + data;
        string_size[i] = copy->string_size[i];
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
        text[i] = NULL;
    }

    if(text)
        delete[] text;
    if(string_size)
        delete[] string_size;

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

    FILE *output = fopen(path, "w");

    assert(output != NULL);

    for(int i = 0; i < n_strings; i ++) {
        fprint_str(string_size[i], text[i], &output);
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
        if(data[i] == '\r') {
            data[i] == '\0';
        }
        if(data[i] == '\n' || data[i] == EOF) {
            data[i] = '\0';
            n_strings ++;
        }
    }

    text = new char*[n_strings];
    string_size = new int[n_strings];

    int str_counter = 1;
    int str_length = 1;

    text[0] = data;

    for(int i = 1; i < data_size && str_counter <= n_strings; i ++) {
        if(data[i] == '\0') {
            text[str_counter] = data + i + 1;
            string_size[str_counter - 1] = str_length;

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
    \brief Sorter by alphabet

    \param  [in]    f                       number of the first element
    \param  [in]    s                       number of the second element
    \param  [out]   a                       pointer to sortered StringArray

    \return is element number f should be after element number s

*/
bool StringArray :: sorter_alphabet(int f, int s, const StringArray *a) {
    if(a->string_size[f] == 0 || (a->string_size[f] == 0 && a->string_size[s] == 0))
        return false;
    else if(a->string_size[s] == 0)
        return true;

    int counter_f = 0, counter_s = 0;
    int l_counter_f = 0, l_counter_s = 0;

    while(counter_f < a->string_size[f] && counter_s < a->string_size[s]) {
        while(!((*(a->text[f] + counter_f) >= 'a' && *(a->text[f] + counter_f) <= 'z') || (*(a->text[f] + counter_f) >= 'A' && *(a->text[f] + counter_f) <= 'Z') || *(a->text[f] + counter_f) == '\0')){
            counter_f ++;
        }
        while(!((*(a->text[s] + counter_s) >= 'a' && *(a->text[s] + counter_s) <= 'z') || (*(a->text[s] + counter_s) >= 'A' && *(a->text[s] + counter_s) <= 'Z') || *(a->text[s] + counter_s) == '\0')){
            counter_s ++;
        }

        if(*(a->text[f] + counter_f) == '\0') {
            if(l_counter_f == 0)
                l_counter_f = NO_LETTER;
            break;
        }
        if(*(a->text[s] + counter_s) == '\0') {
            if(l_counter_s == 0)
                l_counter_s = NO_LETTER;
            break;
        }

        l_counter_f = counter_f;
        l_counter_s = counter_s;

        if(*(a->text[f] + counter_f) > *(a->text[s] + counter_s)) {
            return true;
        } else if(*(a->text[f] + counter_f) < *(a->text[s] + counter_s)) {
            return false;
        }

        counter_f ++;
        counter_s ++;
    }
    if(l_counter_f == NO_LETTER || (l_counter_f == NO_LETTER && l_counter_s == NO_LETTER))
        return false;
    else if(l_counter_s == NO_LETTER)
        return true;
    if(*(a->text[f] + l_counter_f) == *(a->text[s] + l_counter_s)) {
        return false;
    }

    assert(false);
}

/*!
    \brief Sorter by end of the strings by alphabet

    \param  [in]    f                       number of the first element
    \param  [in]    s                       number of the second element
    \param  [out]   a                       pointer to sortered StringArray

    \return is element number f should be after element number s

*/
bool StringArray :: sorter_rhyme(int f, int s, const StringArray *a) {
    if(a->string_size[f] == 0 || (a->string_size[f] == 0 && a->string_size[s] == 0))
        return false;
    else if(a->string_size[s] == 0)
        return true;

    int counter_f = a->string_size[f] - 1, counter_s = a->string_size[s] - 1;
    int l_counter_f = counter_f, l_counter_s = counter_s;

    while(counter_f < a->string_size[f] && counter_s < a->string_size[s]) {
        while(!((*(a->text[f] + counter_f) >= 'a' && *(a->text[f] + counter_f) <= 'z') ||
                (*(a->text[f] + counter_f) >= 'A' && *(a->text[f] + counter_f) <= 'Z') || counter_f < 0)){
            counter_f --;
        }
        while(!((*(a->text[s] + counter_s) >= 'a' && *(a->text[s] + counter_s) <= 'z') ||
                (*(a->text[s] + counter_s) >= 'A' && *(a->text[s] + counter_s) <= 'Z') || counter_s < 0)){
            counter_s --;
        }

        if(counter_f < 0) {
            if(l_counter_f == a->string_size[f] - 1)
                l_counter_f = NO_LETTER;
            break;
        }
        if(counter_s < 0) {
            if(l_counter_s == a->string_size[s] - 1)
                l_counter_s = NO_LETTER;
            break;
        }

        l_counter_f = counter_f;
        l_counter_s = counter_s;

        if(*(a->text[f] + counter_f) > *(a->text[s] + counter_s)) {
            return true;
        } else if(*(a->text[f] + counter_f) < *(a->text[s] + counter_s)) {
            return false;
        }

        counter_f --;
        counter_s --;
    }
    if(l_counter_f == NO_LETTER || (l_counter_f == NO_LETTER && l_counter_s == NO_LETTER))
        return false;
    else if(l_counter_s == NO_LETTER)
        return true;
    if(*(a->text[f] + l_counter_f) == *(a->text[s] + l_counter_s)) {
        return false;
    }

    assert(false);
}

/*!
    \brief Quicksorting

    \param  [in]    first                   number of the first element
    \param  [in]    last                    number of the last  element
    \param  [out]   sortrer                 pointer to sorter function

    \return is element number f should be after element number s

*/
bool StringArray :: sort_text(int first, int last, bool (*sorter)(int, int, const StringArray*)) {
    if(first >= last)
        return true;

    assert(first >= 0);
    assert(first < n_strings);
    assert(last >= 0);
    assert(last < n_strings);

    int left = first, right = last;
    int pivot = left;
    char *pivot_t = text[pivot];
    int pivot_s = string_size[pivot];

     while(left <= right) {
        while(sorter(right, pivot, this)) {
            right --;
        }
        while(sorter(pivot, left, this)) {
            left ++;
        }

        if(left <= right) {
            std::swap(text[left], text[right]);
            std::swap(string_size[left], string_size[right]);
            left++;
            right--;
        }
    }
    if(first < right)
        sort_text(first, right, sorter);
    if(last > left)
        sort_text(left, last, sorter);

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
    char **n_text = new char*[quatrains_num * 4];
    int *n_string_size = new int[quatrains_num * 4];

    for(int i = 0; i < quatrains_num * 4; i ++) {
        n_text[i] = text[string_num[i]];
        n_string_size[i] = string_size[string_num[i]];
    }

    delete_marking();

    n_strings = quatrains_num * 4;
    text = n_text;
    string_size = n_string_size;

    delete[] string_num;

    return true;
}
