/*!
    \file
    \brief String Array declaration
    \author Yaroslav Pozndyak

    File with a declaration of the class StringArray
*/
#include "info.h"

/*!
    \brief Main class

    This class store and chage data.
*/
class StringArray {
public:
    StringArray();
    StringArray(char *data);
    StringArray(StringArray*);
    ~StringArray();

    char *data;             ///< store data
    int data_size;          ///< size of data

    char* *text;            ///< pointers to begin of strings
    int *string_size;       ///< size of strings
    int n_strings;          ///< number of strings

    bool scan(const char*);
    bool print(const char *);
    bool mark_text();

    bool sort_text(int, int, bool (*sorter)(int, int, StringArray*));

    bool clean_mem();

    static bool sorter_alphabet(int, int, StringArray*);
    static bool sorter_rhyme(int, int, StringArray*);

    bool is_marked;
    bool is_data_loaded;

    static const int NO_LETTER = -1;
private:
    bool delete_marking();
    bool delete_data();
};
