#include "info.h"

static const int LEFT = 1;
static const int RIGHT = 2;
static const int PARENT = 3;

class Node {
public:
    char* data;
    int data_len;

    Node* left;
    Node* right;
    Node* par;

    int id;
    int type;

    int val;

    static const int TYPE_DIG = -3;
    //static const int TYPE_VAR = 1;

    static const int TYPE_CLR = -1;


    static const int TYPE_DEC_VAR = 0;
    static const int TYPE_VAR = 1;

    static const int TYPE_DEC_FU = 2;
    static const int TYPE_FU = 3;
    static const int TYPE_ARG = 4;
    static const int TYPE_DEC_ARG = 5;

    static const int TYPE_CODE = 6;

    static const int TYPE_ASS = 7;
    static const int TYPE_OPR = 8;

    static const int TYPE_LET = 9;

    static const int TYPE_CALL = 10;
    static const int TYPE_FREE_CALL = 11;

    static const int TYPE_IF = 12;

    static const int TYPE_RET = 13;

    static const int TYPE_PRI = 14;
    static const int TYPE_SCA = 15;

    static const int TYPE_SQRT = 16;

    static const int TYPE_WH = 17;
};

class Tree {
public:
    char *data;
    Node* nodes;

    Node* root;

    long long nodes_num;
    long long nodes_size;
    static const long long MIN_NODES_SIZE = 1000;
    static const long long DATA_SIZE = 10000;

    long long data_size;
};

void NodeCtor(Node*);
void NodeCtor(Node*, char*, int, Node*, Node*, Node*, int);
void NodeDtor(Node*);

void TreeCtor(Tree*);
void TreeDtor(Tree*);

Node* get_cnt(const Node*, int);
void set_cnt(Node*, int, Node*);

int get_way_flag(char*, char*);
int TreeGoDown(Tree*, Node**, char*, int*);
Node* TreePush(Tree*, char*, int);

void NodeDumb(Node*);
void TreeDumb(Tree*);

void NodeInfDumb(Node*);
void TreeInfDumb(Tree*);

void TreePostDumb(Tree*);
void NodePostDumb(Node*);

void TreeAssDumb(Tree*, FILE*);
void NodeAssDumb(Node*, FILE*);

void TreeCreate(Tree*, char*);

void NodeDelete(Node*);

int NodeNumbering(Node*, int, int*);

Node* searchDeclarationEarlier(Node*, int, char*, int);

Node* TreeNodeCreate(Tree*, int, char*, int, Node*, Node*);
