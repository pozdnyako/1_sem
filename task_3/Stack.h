/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    File with description of the struct Stack
*/

#include "info.h"

#define ASSERT_OK(s) if(!StackOk(&s)) {StackDumb(&s, #s); assert(0);}

/*!
    \brief Stack
*/
struct Stack{
    long long data_size;                ///<It is the size of used part of the stack
    long long data_capacity;            ///<It is the size of the stack
    stack_e *data;                          ///<It is the pointer to the first element of data that is stored

    static const int DATA_ERROR = -1;   ///<It is the constant for returning and data_capacity == DATA_ERROR if Stack isn't constructed.

    Stack()
    :data_size(0)
    ,data_capacity(DATA_ERROR)
    ,data(NULL)
    {}
};

void StackCtor(Stack*, long long);
void StackDtor(Stack*);
int StackPush(Stack*, stack_e);
stack_e StackPop(Stack*);

bool StackOk(Stack*);
void StackDumb(Stack*, const char*);
