#include "info.h"

typedef double t_data;
typedef int ptr;

const int MIN_LIST_LENGTH = 5;

const ptr LIST_TAIL = -100;
const ptr LIST_HEAD = -200;

const ptr LIST_CLR = -3;

const ptr LIST_END = -4;

const ptr N_PTR = -1;


struct Element {
    ptr next;
    ptr prev;

    t_data *data;
};

struct List {
    ptr head;
    ptr tail;
    ptr free;

    bool is_sorted;
    int length;

    int max_length;

    Element* elm;
};

void ElmCtor(Element*);


void ListCtor(List*);
void ListDtor(List*);

ptr ListInsert(List*, const ptr, t_data*);
ptr ListPushBack(List*, t_data*);
int ListRemove(List*, const ptr);

int ListIsOk(List*);

const int DONT_FIND = -1;
int ListFind(List*, int);

void ListDumb(List*);
