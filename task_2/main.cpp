/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    Main file with main() and description of other global functions.
*/
#include "StringArray.h"

/*!
    \brief Input
    \param  [in]    path                pointer to path to input file
    \param  [out]   buf_size            pointer to size of file
    \param  [out]   buf                 pointer to data

    \return sucsess

    \note Delete buf and buf_size. Scan file to buf.
*/
bool scan_str_with_delete_and_new(const char* path, int *buf_size, char* *buf) {
    assert(path != NULL);
    assert(buf_size != NULL);
    assert(buf != NULL);

    FILE* input = fopen(path, "r");

    assert(input != NULL);

    fseek(input, 0, SEEK_END);
    int size = ftell(input) / sizeof(char);
    size ++;        // It is only with the heart that one can see rightly; what is essential is invisible to the eye.

    fseek(input, 0, SEEK_SET);

    *buf_size = 0;
    delete *buf;

    *buf_size = size;
    *buf = new char[size];

    int counter = 0;

    for(;counter < size;counter++) {
        char c = getc(input);

        (*buf)[counter] = c;

        if(c == EOF) {
            size = counter + 1;
            break;
        }
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
        printf("%c", str[i]);
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
bool fprint_str(int str_size, char *str, FILE* *file) {
    assert(str != NULL);

    for(int i = 0; i < str_size; i ++) {
        fprintf(*file, "%c", str[i]);
    }
    return true;
}

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");
    StringArray str_a;

    char* path_input = "input/1.txt";
    char* path_output_alphabet = "output/alphabet.txt";
    char* path_output_rhyme = "output/rhyme.txt";

    switch(argc) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        path_input = argv[1];
        break;
    case 3:
        path_input = argv[1];
        path_output_alphabet = argv[2];
        break;
    default:
        path_input = argv[1];
        path_output_alphabet = argv[2];
        path_output_rhyme = argv[3];
        break;
    }

    printf("[INFO]\t\tinput: \t\t\t\t%s\n[INFO]\t\toutput sorted by alphabet: \t%s\n[INFO]\t\toutput sorted by rhyme: \t%s\n", path_input, path_output_alphabet, path_output_rhyme);

    bool is_scaned = str_a.scan(path_input);
    if(!is_scaned) {
        printf("[ERROR]\tscaning\n");
        return -1;
    }
    printf("[COMPLETE]\tscaning\n");

    bool is_marked = str_a.mark_text();
    if(!is_marked) {
        printf("[ERROR]\tmarking text\n");
        return -1;
    }
    printf("[COMPLETE]\tmarking text\n");


    StringArray str_a_alphabet(&str_a);
    bool is_sorted_alphabet = str_a_alphabet.sort_text(0, str_a_alphabet.n_strings - 1, StringArray::sorter_alphabet);
    if(!is_sorted_alphabet) {
        printf("[ERROR]\tsorting by alphabet\n");
        return -1;
    }

    StringArray str_a_rhyme(&str_a);
    bool is_sorted_rhyme = str_a_rhyme.sort_text(0, str_a_rhyme.n_strings - 1, StringArray::sorter_rhyme);
    if(!is_sorted_rhyme) {
        printf("[ERROR]\tsorting by alphabet from end\n");
        return -1;
    }
    printf("[COMPLETE]\tsorting\n");

    bool is_printed_alphabet = str_a_alphabet.print(path_output_alphabet);
    bool is_printed_rhyme = str_a_rhyme.print(path_output_rhyme);

    if(!is_printed_alphabet) {
        printf("[ERROR]\tprinting strings that are sorted by alphabet\n");
        return -1;
    }
    if(!is_printed_rhyme) {
        printf("[ERROR]\tprinting strings that are sorted by alphabet from end\n");
        return -1;
    }
    printf("[COMPLETE]\tprinting\n");

    str_a.StringArray::~StringArray();
    str_a_alphabet.StringArray::~StringArray();
    str_a_rhyme.StringArray::~StringArray();
    printf("[COMPLETE]\tmemory cleaning");
    return 0;
}
