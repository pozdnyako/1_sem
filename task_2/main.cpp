/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    Main file with main() and description of other global functions.
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
bool scan_str_with_delete_and_new(const char* path, int *buf_size, char* *buf) {
    assert(path != NULL);
    assert(buf_size != NULL);
    assert(buf != NULL);

    FILE* input = fopen(path, "rb");
    assert(input != NULL);

    int size = get_file_size(&input);
    int length = size / sizeof(char);

    *buf_size = 0;
    delete *buf;

    *buf_size = length + 1;
    *buf = new char[length + 1];
    int result = fread(*buf, sizeof(char), size, input);
    *(*buf + length) = EOF;

    if(result != size ) {
        printf("[ERROR] reading from %s error", path);
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

/*!
    \brief Use console arguments
    \param  [in]    argc                    number of the argumnets
    \param  [in]    argv                    array of arguments
    \param  [out]   path_input              pointer to the string with path to input file
    \param  [out]   path_output_alphabet    pointer to the string with path to output file for sortered by alphabet text
    \param  [out]   path_output_rhyme       pointer to the string with path to output file for sortered by rhyme text
    \param  [out]   path_output_standard    pointer to the string with path to output file for standard text
    \param  [out]   path_output_sonnet      pointer to the string with path to outout file for sonnet 2.0
    \param  [out]   new_sonnet_length       pointer to the length of the sonnet 2.0 on quatrains

    \return sucsess
*/
bool use_console_arg(int argc, char* argv[], char* *path_input, char* *path_output_alphabet,
                     char* *path_output_rhyme, char* *path_output_standard, char* *path_output_sonnet, int *new_sonnet_length) {
    switch(argc) {
    case 0:
        break;
    case 1:
        break;
    case 2:
        *path_input = argv[1];
        break;
    case 3:
        *path_input = argv[1];
        *path_output_alphabet = argv[2];
        break;
    case 4:
        *path_input = argv[1];
        *path_output_alphabet = argv[2];
        *path_output_rhyme = argv[3];
    case 5:
        *path_input = argv[1];
        *path_output_alphabet = argv[2];
        *path_output_rhyme = argv[3];
        *path_output_standard = argv[4];
        break;
    case 6:
        *path_input = argv[1];
        *path_output_alphabet = argv[2];
        *path_output_rhyme = argv[3];
        *path_output_standard = argv[4];
        *path_output_sonnet = argv[5];
        break;
    default:
        *path_input = argv[1];
        *path_output_alphabet = argv[2];
        *path_output_rhyme = argv[3];
        *path_output_standard = argv[4];
        *path_output_sonnet = argv[5];
        *new_sonnet_length = atoi(argv[6]);
        break;
    }

    return true;
}

int main(int argc, char* argv[]) {
    srand(time(NULL));
    setlocale(LC_ALL, "Russian");
    StringArray str_a;

    char* path_input = "input/1.txt";
    char* path_output_alphabet = "output/alphabet.txt";
    char* path_output_rhyme = "output/rhyme.txt";
    char* path_output_standard = "output/standard.txt";
    char* path_output_sonnet = "output/sonnet2_0.txt";

    int new_sonnet_length;

    use_console_arg(argc, argv, &path_input, &path_output_alphabet, &path_output_rhyme, &path_output_standard, &path_output_sonnet, &new_sonnet_length);

    printf("[INFO]\t\tinput: \t\t\t\t%s\n", path_input);
    printf("[INFO]\t\toutput sorted by alphabet: \t%s\n", path_output_alphabet);
    printf("[INFO]\t\toutput sorted by rhyme: \t%s\n", path_output_rhyme);
    printf("[INFO]\t\toutput standard: \t\t%s\n", path_output_standard);
    printf("[INFO]\t\toutput sonnet 2.0: \t\t%s\n", path_output_sonnet);
    if(argc < 7)
        printf("[INFO]\t\tlength of the new sonnet didn't get as console argument\n");

    bool is_scaned = str_a.scan(path_input);
    if(!is_scaned)                      {    printf("[ERROR]\tscaning\n");                                              return -1;  }
    printf("[COMPLETE]\tscaning\n");

    bool is_marked = str_a.mark_text();
    if(!is_marked)                      {    printf("[ERROR]\tmarking text\n");                                         return -1;  }
    printf("[COMPLETE]\tmarking text\n");


    StringArray str_a_alphabet(&str_a);
    bool is_sorted_alphabet = str_a_alphabet.sort_text(0, str_a_alphabet.n_strings - 1, StringArray::sorter_alphabet);
    if(!is_sorted_alphabet)             {   printf("[ERROR]\tsorting by alphabet\n");                                   return -1;  }

    StringArray str_a_rhyme(&str_a);
    bool is_sorted_rhyme = str_a_rhyme.sort_text(0, str_a_rhyme.n_strings - 1, StringArray::sorter_rhyme);
    if(!is_sorted_rhyme)                {  printf("[ERROR]\tsorting by alphabet from end\n");                           return -1;  }

    printf("[COMPLETE]\tsorting\n");

    printf("[INPUT]\twirte a length of the new sonnet in quatrains: ");
    scanf("%d", &new_sonnet_length);
    bool is_correct_new_sonnet_length = (new_sonnet_length >= 0);
    if(!is_correct_new_sonnet_length)   {   printf("[ERROR]\tuncorrect length of the new sonnet\n");                    return -1;  }

    StringArray str_a_sonnet(&str_a_rhyme);
    bool is_mixed = str_a_sonnet.generate_sonnet(10);
    if(!is_mixed)                       {   printf("[ERROR]\tgeneration sonnet 2.0\n");                                             }

    printf("[COMPLETE]\tgeneration sonnet 2.0\n");


    bool is_printed_alphabet = str_a_alphabet.print(path_output_alphabet);
    if(!is_printed_alphabet)            {   printf("[ERROR]\tprinting strings that are sorted by alphabet\n");          return -1;  }

    bool is_printed_rhyme = str_a_rhyme.print(path_output_rhyme);
    if(!is_printed_rhyme)               {   printf("[ERROR]\tprinting strings that are sorted by alphabet from end\n"); return -1;  }

    bool is_printed_standard = str_a.print(path_output_standard);
    if(!is_printed_standard)            {   printf("[ERROR]\tprinting used strings\n");                                 return -1;  }

    bool is_printed_sonnet = str_a_sonnet.print(path_output_sonnet);
    if(!is_printed_sonnet)              {   printf("[ERROR]\tprinting sonnet\n");                                       return -1;  }

    printf("[COMPLETE]\tprinting\n");

    str_a.StringArray::~StringArray();
    str_a_alphabet.StringArray::~StringArray();
    str_a_rhyme.StringArray::~StringArray();
    str_a_sonnet.StringArray::~StringArray();
    printf("[COMPLETE]\tmem_cleaning\n");
    return 0;
}
