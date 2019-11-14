#include "token.h"

Token :: Token() {
    type = type_error;
    data_len = 0;
    val = 0;
}

Token :: ~Token() {
}

int Token :: skipIgnorSymbols(char* cur) {
    char* start = cur;

    #define DEF_IGNORE(symbol) if(*cur == symbol) { cur ++; num ++; }
    #define DEF_SEPARATOR(...)
    #define DEF_OPERATOR(...)
    int num = 0;
    do{
        num = 0;
        #include "symbol.h"
    } while(num);
    #undef DEF_IGNORE
    #undef DEF_SEPARATOR
    #undef DEF_OPERATOR

    return cur - start;
}

#define res_let res[0]
#define res_id  res[1]
#define res_key res[2]
#define res_sep res[3]
#define res_opr res[4]
#define res_cmt res[5]

#define check_res \
    for(int i = 0; i < TOKEN_TYPE_NUM; i ++) {\
        if(res[i] > 0) return res[i];\
    }

#define res_to_zero\
    for(int i = 0; i < TOKEN_TYPE_NUM; i ++) {\
        res[i] = 0;\
    }

int Token :: getToken(char* cur, Token* _token) {
    char* start = cur;

    int res[TOKEN_TYPE_NUM]; res_to_zero

    res_let += getLeteral  (cur, _token);
    res_id  += getId       (cur, _token);
    res_key += getKeyword  (cur, _token);
    res_sep += getSeparator(cur, _token);
    res_opr += getOperator (cur, _token);
    res_cmt += getComment  (cur, _token);

    if(res_id > 0 && res_key > 0) {
        if(res_key == res_id)
            return getKeyword(cur, _token);
        else
            return getId(cur, _token);
    }

    if(res_opr > 0 && res_cmt > 0) {
        return getComment(cur, _token);
    }

    check_res

    return 0;
}

#undef check_res



#define _check_res(m_type)  \
    if(res > 0) {\
        _token->data_len = res;\
        _token->data = start;\
        _token->type = type_##m_type;\
        return res;\
    }

/// ------------------------------------- LETERAL -------------------------------------
#define check_res _token->val = 0; _check_res(leteral)
int Token :: getLeteral(char* cur, Token* _token) {
    char* start = cur;
    int res = 0;

    res = getInt(cur);
    check_res

    return 0;
}

#undef check_res
/// ------------------------------------- IDENTEFICATOR -------------------------------------
#define check_res _token->val = 0; _check_res(id)
int Token :: getId(char* cur, Token* _token) {
    char* start = cur;

    int res = getWord(cur);
    if(res) {
        cur += res;

        int delta_res = 0;
        do{
            delta_res = 0;

            delta_res += getInt(cur);
            delta_res += getWord(cur);

            cur += delta_res;
            res += delta_res;
        } while(delta_res);
    }

    check_res
    return 0;
}
#undef check_res
/// ------------------------------------- KEYWORD -------------------------------------
#define check_res _check_res(keyword)
int Token :: getKeyword(char* cur, Token* _token) {
    char* start = cur;

    int res = getWord(cur);

    if(res == 0)
        return res;

    #define DEF_KEY(name, num) \
        if(strStartCmp(start, #name)) { _token->val = num; check_res }

    #include "keywords.h"

    #undef GET_KEY

    return 0;
}
#undef check_res

/// ------------------------------------- SEPARATOR -------------------------------------
#define check_res _check_res(separator)
int Token :: getSeparator(char* cur, Token* _token) {
    char* start = cur;
    int res = 0;

    #define DEF_IGNORE(...)
    #define DEF_SEPARATOR(symbol) \
        if(*cur == symbol) { _token->val = *cur; res = 1; cur++; check_res  }
    #define DEF_OPERATOR(...)

    #include "symbol.h"

    #undef DEF_IGNORE
    #undef DEF_SEPARATOR
    #undef DEF_OPERATOR

    return 0;

}
#undef check_res

/// ------------------------------------- OPERATOR -------------------------------------
#define check_res _check_res(operator)
int Token :: getOperator(char* cur, Token* _token) {
    char* start = cur;
    int res = 0;

    #define DEF_IGNORE(...)
    #define DEF_SEPARATOR(...)
    #define DEF_OPERATOR(symbol) \
        if(*cur == symbol) { _token->val = *cur; res = 1; cur++; check_res  }

    #include "symbol.h"

    #undef DEF_IGNORE
    #undef DEF_SEPARATOR
    #undef DEF_OPERATOR

    return 0;

}
#undef check_res

/// ------------------------------------- COMMENT -------------------------------------
#define check_res _check_res(comment)
int Token :: getComment(char* cur, Token* _token) {
    char* start = cur;
    int res = 0;

    if(strStartCmp(start, "//")) {
        cur += 2;
        res += 2;

        while(*cur != '\0' && *cur != '\n') {
            cur ++;
            res ++;
        }

        check_res
    }

    if(strStartCmp(start, "/*")) {
        cur += 2;
        res += 2;

        while(!strStartCmp(cur, "*/")) {
            cur ++;
            res ++;

            if(*cur == '\0'){
                break;
            }
        }

        if(*cur != '\0'){
            cur += 2;
            res += 2;
        }

        check_res
    }

    return 0;

}
#undef check_res

#undef _check_res

int Token :: getInt(char* cur) {
    char* start = cur;

    while(*cur >= '0' && *cur <= '9'){
        cur ++;
    }

    return cur - start;
}

int Token :: getWord(char* cur) {
    char* start = cur;

    while(getLetter(cur) || *cur == '_') {
        cur ++;
    }

    return cur - start;
}

int Token :: getLetter(char* cur) {
    if((*cur >= 'a' && *cur <= 'z') ||
       (*cur >= 'A' && *cur <= 'Z') ||
       (*cur >= 'à' && *cur <= 'ÿ') ||
       (*cur >= 'À' && *cur <= 'ß'))
        return 1;

    return 0;
}


void Token :: Dumb() {
    #define DEF_TYPE(name, num) \
        if(type == num) {\
            printf("([%9s], %4d, ", #name, val);\
            for(int i = 0; i < data_len; i ++) {\
                printf("%c", data[i]);\
            }\
            printf(")\n");\
        }
    #include "tokens_type.h"
    #undef DEF_TYPE
}
