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
bool scan_str_with_delete_and_new(const char* path, size_t *buf_size, char* *buf) {
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
        printf("[ERROR] reading from %s", path);
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

int qsorter_alphabet(const void *f, const void *s) {
    //std::cout << f << " " << s << std::endl;

    char* str_f = ((M_str*)f)->str;
    char* str_s = ((M_str*)s)->str;

    int len_f = ((M_str*)f)->len;
    int len_s = ((M_str*)s)->len;

    //std::cout << len_f << " " << len_s << std::endl;

    if(len_f == 0 || (len_f == 0 && len_s == 0))
        return -1;
    else if(len_s == 0)
        return 1;

    int counter_f = 0, counter_s = 0;
    int l_counter_f = 0, l_counter_s = 0;

    while(counter_f < len_f && counter_s < len_s) {
        while(!((*(str_f + counter_f) >= 'a' && *(str_f + counter_f) <= 'z') || (*(str_f + counter_f) >= 'A' && *(str_f + counter_f) <= 'Z') || *(str_f + counter_f) == '\0')){
            counter_f ++;
        }
        while(!((*(str_s + counter_s) >= 'a' && *(str_s + counter_s) <= 'z') || (*(str_s + counter_s) >= 'A' && *(str_s + counter_s) <= 'Z') || *(str_s + counter_s) == '\0')){
            counter_s ++;
        }

        if(*(str_f + counter_f) == '\0') {
            if(l_counter_f == 0)
                l_counter_f = NO_LETTER;
            break;
        }
        if(*(str_s + counter_s) == '\0') {
            if(l_counter_s == 0)
                l_counter_s = NO_LETTER;
            break;
        }

        l_counter_f = counter_f;
        l_counter_s = counter_s;

        if(*(str_f + counter_f) > *(str_s + counter_s)) {
            return 1;
        } else if(*(str_f + counter_f) < *(str_s + counter_s)) {
            return -1;
        }

        counter_f ++;
        counter_s ++;
    }
    if(l_counter_f == NO_LETTER || (l_counter_f == NO_LETTER && l_counter_s == NO_LETTER))
        return -1;
    else if(l_counter_s == NO_LETTER)
        return 1;
    if(*(str_f + l_counter_f) == *(str_s + l_counter_s)) {
        return -1;
    }

    assert(false);
}

int qsorter_rhyme(const void *f, const void *s) {
    char* str_f = ((M_str*)f)->str;
    char* str_s = ((M_str*)s)->str;

    int len_f = ((M_str*)f)->len;
    int len_s = ((M_str*)s)->len;

    if(len_f == 0 || (len_f == 0 && len_s == 0))
        return -1;
    else if(len_s == 0)
        return 1;

    int counter_f = len_f - 1, counter_s = len_s - 1;
    int l_counter_f = counter_f, l_counter_s = counter_s;

    while(counter_f < len_f && counter_s < len_s) {
        while(!((*(str_f + counter_f) >= 'a' && *(str_f + counter_f) <= 'z') ||
                (*(str_f + counter_f) >= 'A' && *(str_f + counter_f) <= 'Z') || counter_f < 0)){
            counter_f --;
        }
        while(!((*(str_s + counter_s) >= 'a' && *(str_s + counter_s) <= 'z') ||
                (*(str_s + counter_s) >= 'A' && *(str_s + counter_s) <= 'Z') || counter_s < 0)){
            counter_s --;
        }

        if(counter_f < 0) {
            if(l_counter_f == len_f - 1)
                l_counter_f = NO_LETTER;
            break;
        }
        if(counter_s < 0) {
            if(l_counter_s == len_s - 1)
                l_counter_s = NO_LETTER;
            break;
        }

        l_counter_f = counter_f;
        l_counter_s = counter_s;

        if(*(str_f + counter_f) > *(str_s + counter_s)) {
            return 1;
        } else if(*(str_f + counter_f) < *(str_s + counter_s)) {
            return -1;
        }

        counter_f --;
        counter_s --;
    }
    if(l_counter_f == NO_LETTER || (l_counter_f == NO_LETTER && l_counter_s == NO_LETTER))
        return -1;
    else if(l_counter_s == NO_LETTER)
        return 1;
    if(*(str_f + l_counter_f) == *(str_s + l_counter_s)) {
        return -1;
    }

    assert(false);
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
        *path_output_standard = argv[4];
        *path_output_sonnet = argv[5];
        *path_output_rhyme = argv[3];
        break;
    default:
        {
        int arg = 1;
        *path_input           =      argv[arg++];
        *path_output_alphabet =      argv[arg++];
        *path_output_rhyme    =      argv[arg++];
        *path_output_standard =      argv[arg++];
        *path_output_sonnet   =      argv[arg++];
        *new_sonnet_length    = atoi(argv[arg++]);
        }
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

    int new_sonnet_length = 0;

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
    bool is_sorted_alphabet = str_a_alphabet.sort_text(0, str_a_alphabet.n_strings - 1, qsorter_alphabet);
    if(!is_sorted_alphabet)             {   printf("[ERROR]\tsorting by alphabet\n");                                   return -1;  }

    StringArray str_a_rhyme(&str_a);
    bool is_sorted_rhyme = str_a_rhyme.sort_text(0, str_a_rhyme.n_strings - 1, qsorter_rhyme);
    if(!is_sorted_rhyme)                {  printf("[ERROR]\tsorting by alphabet from end\n");                           return -1;  }

//  str_a_alphabet.sort_text(0, str_a_alphabet.n_strings - 1, qsorter_alphabet) || ({ return 4; 0; });

    printf("[COMPLETE]\t" "sorting\n");

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

    return 0;
}
