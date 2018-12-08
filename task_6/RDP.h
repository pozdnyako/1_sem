#include "info.h"

class Calc {
public:

    Calc();
    ~Calc();

    Tree m_tree;

    Node* getG(char*);
    Node* getE();
    Node* getT();
    Node* getPo();
    Node* getP();
    Node* getN();
    Node* getId();
    Node* getFu();

    static const int MIN_MSG_SIZE = 10;

    static const short N_ERROR = 1;
    static const short END_ERROR = 2;
    static const short E_ERROR = 3;
    static const short T_ERROR = 4;
    static const short P_ERROR = 5;
    static const short ID_ERROR = 6;
    static const short FU_ERROR = 6;

private:

    void msg_push(short);
    void msg_print();
    void msg_clr();
    short msg_front();
    short msg_pop();
    bool msg_is_emp();
    int msg_size, msg_max_size;

    char* cur;
    short* msg;
};
