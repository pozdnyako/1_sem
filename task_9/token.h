#include "info.h"

class Token {
public:
    Token();
    virtual ~Token();

    int type;
    #define DEF_TYPE(name, num) \
    static const int type_##name = num;
    #include "tokens_type.h"
    #undef DEF_TYPE

    char* data;
    int data_len;

    static int skipIgnorSymbols(char*);

    static int getToken(char*, Token*);

    static int getLeteral  (char*, Token*);
    static int getId       (char*, Token*);
    static int getKeyword  (char*, Token*);
    static int getSeparator(char*, Token*);
    static int getOperator (char*, Token*);
    static int getComment  (char*, Token*);

    static int getInt(char*);
    static int getWord(char*);
    static int getLetter(char*);

    static const int TOKEN_TYPE_NUM = 6;

    int val;

    void Dumb();
};
