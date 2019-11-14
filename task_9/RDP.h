#include "info.h"

class Parser {
public:

    Parser();
    ~Parser();

    Tree *m_tree;

    Node* getG(Token*);
    Node* getM();
    Node* getDecVar();
    Node* getDecFu();
    Node* getFu();
    Node* getFuArgs();
    Node* getFuArg();
    Node* getCode();
    Node* getVar();
    Node* getAss();

    Node* getE();

    Node* getOr();
    Node* getAnd();
    Node* getOrd();
    Node* getS();
    Node* getT();
    Node* getP();

    Node* getPri();
    Node* getSca();

    Node* getSqrt();

    Node* getLet();
    Node* getCall();
    Node* getFreeCall();

    Node* getIf();
    Node* getWh();
    Node* getLogic();

    Node* getRet();

    static const int MIN_MSG_SIZE = 10;

    static const short END_ERROR = 1;
    static const short A_ERROR = 2;
    static const short P_ERROR = 2;

private:

    void msg_push(short);
    void msg_print();
    void msg_clr();
    short msg_front();
    short msg_pop();
    bool msg_is_emp();
    int msg_size, msg_max_size;

    Token* cur;
    short* msg;
};
