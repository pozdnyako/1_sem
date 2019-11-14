#include "info.h"

static const int LEFT = 1;
static const int RIGHT = 2;
static const int PARENT = 3;

struct Node {
    t_data* data;
    Node* left;
    Node* right;
    Node* par;

    int id;
    int type;

    static const int TYPE_DIG = 0;
    static const int TYPE_OPR = 1;
    static const int TYPE_VAR = 2;
    static const int TYPE_CLR = -1;
};

struct Tree {
    int *data;
    Node* nodes;

    Node* root;

    long long nodes_num;
    long long nodes_size;
    static const long long MIN_NODES_SIZE = 1000;
    static const long long DATA_SIZE = 1000;

    long long data_size;
};

void NodeCtor(Node*);
void NodeCtor(Node*, t_data*, Node*, Node*, Node*, int);
void NodeDtor(Node*);

void TreeCtor(Tree*);
void TreeDtor(Tree*);

Node* get_cnt(const Node*, int);
void set_cnt(Node*, int, Node*);

int get_way_flag(t_data*, t_data*);
int TreeGoDown(Tree*, Node**, t_data*, int*);
Node* TreePush(Tree*, t_data*);

void NodeDumb(Node*);
void TreeDumb(Tree*);

void NodeInfDumb(Node*);
void TreeInfDumb(Tree*);

void TreeCreate(Tree*, char*);

Node *Diff(Tree*, const Node*);

void TreeTexDumb(Tree*, FILE*);

void NodeDelete(Node*);

int NodeDelete0MultSmth(Node*);
int NodeDelete1MultSmth(Node*);
int NodeDelete0PlusSmth(Node*);
int NodeDeleteSmthMin0(Node*);
int NodeDelete0DivSmth(Node*);

Node* TreeNodeCreate(Tree*, int, int, Node*, Node*);
