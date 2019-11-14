#include "info.h"

struct FuInfo{
    Node* m_node;
    int arg_n;

    int var_n_min;
    int var_n_max;
};

struct VarInfo{
    FuInfo *m_fu;
    Node* m_node;
    int n;
};

class AST {
public:
    AST();
    virtual ~AST();

    char* source;

    Token *token;
    Tree m_tree;
    Parser m_parser;

    int main_n;

    int useLexer();
    int useParser();
    int useChecker();
    int useTranslator(const char*);

    int numberingIf();
    int numberingVars();
    int checkVars();
    int checkFuncs();

    int connectVars();

    void deleteTokens();
    void initSource(const char*);
    void deleteSource();

    void DumbTokens();
private:
    FuInfo *funcs;
    int n_func;

    VarInfo *vars;
    int n_var;

    int n_global_vars = 0;

    Token* addToken(Token);

    size_t token_n;
    size_t max_token_n;
    size_t source_len;

    static const size_t MIN_MAX_TOKEN_N = 10;
    static const int MAX_GLOBAL_VARS_NUM = 3;
};
