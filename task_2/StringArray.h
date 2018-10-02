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
    StringArray(StringArray*);
    ~StringArray();

    char *data;             ///< store data
    size_t data_size;       ///< size of data

    size_t n_strings;       ///< number of strings

    M_str *strings;         ///< pointer to begins of strings and them lengths

    bool scan(const char*);
    bool print(const char *);
    bool mark_text();

    bool generate_sonnet(int);

    bool sort_text(int, int, int (*sorter)(const void*, const void*));

    bool clean_mem();
    bool memcopy(StringArray*);

    bool is_marked;         ///< is text marked - using for prevent deleting empty dynamic arrays text and string_size
    bool is_data_loaded;    ///< is text marked - using for prevent deleting empty dynamic array data

    static const int DELTA_STRING = 11;

private:
    bool delete_marking();
    bool delete_data();
};
