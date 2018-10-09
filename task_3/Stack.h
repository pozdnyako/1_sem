#include "info.h"

#define ASSERT_OK(s) if(!StackOk(&s)) {StackDumb(&s, #s); assert(0);}

struct Stack{
    long long data_size;
    long long data_capacity;
    int *data;

    static const int DATA_ERROR = -1;

    Stack()
    :data_size(0)
    ,data_capacity(DATA_ERROR)
    ,data(NULL)
    {}
};

void StackCtor(Stack*, long long);
void StackDtor(Stack*);
int StackPush(Stack*, int);
int StackPop(Stack*);

bool StackOk(Stack*);
void StackDumb(Stack*, const char*);
