/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    File with description of functions that work with Stack
*/

#include "Stack.h"


/*!
    \brief Stack Constructor
    \param  [out]   s                   pointer to stack that is constructing
    \param  [in]    data_capacity       capacity of new stack

    \note It allocate memory for stack
*/
void StackCtor(Stack *s, long long data_capacity) {
    assert(s);

    assert(!s->data);

    s->data = (stack_e*)calloc(data_capacity, sizeof(stack_e));
    s->data_capacity = data_capacity;
    s->data_size = 0;
}

/*!
    \brief Stack Destructor
    \param  [out]   s                   pointer to stack that is destructing

    \note It clean memory for stack
*/
void StackDtor(Stack *s) {
    assert(s);

    if(!(s->data)) {
        printf("[ERROR] data is NULL in StackDtor(Stack*)\n");
    } else{
        free(s->data);
    }

    s->data = NULL;
    s->data_capacity = Stack::DATA_ERROR;
    s->data_size = 0;
}

/*!
    \param  [out]   s                   pointer to stack which the value is pushing
    \param  [in]    val                 value for pushing

    \return -1 if stack is full

    \note Push elemenet to stack
*/
int StackPush(Stack *s, stack_e val) {
    ASSERT_OK(*s);

    if(!(s->data_size + 1 <= s->data_capacity)) {
        return Stack::DATA_ERROR;
    }
    *(s->data + s->data_size) = val;
    s->data_size ++;

    ASSERT_OK(*s);
}

/*!
    \param  [out]   s                   pointer to stack which from the last element is removing

    \return the last element value or -1 if stack is full

    \note Remove last elemenet from stack
*/
stack_e StackPop(Stack* s) {
    ASSERT_OK(*s);

    if(!(s->data_size > 0)) {
        return Stack::DATA_ERROR;
    }

    s->data_size --;

    stack_e val = *(s->data + s->data_size);
    *(s->data + s->data_size) = 0;

    ASSERT_OK(*s);

    return val;
}

/*!
    \param  [in]    s                   pointer to stack that is checking

    \return is stack ok
*/
bool StackOk(Stack *s) {
    return s &&
           s->data &&
          (s->data_size >= 0 && s->data_size <= s->data_capacity) &&
          (s->data_capacity != Stack::DATA_ERROR);
}

/*!
    \brief  Print info about stack

    \param  [in]    s                   pointer to stack that is printing
    \param  [in]    nm                  name of the stack
*/
void StackDump(Stack *s, const char* nm) {
    printf("\tStack %s[%p] ", nm, s);

    if(StackOk(s))
        printf("(OK)");
    else
        printf("(ERROR)");

    if(s->data) {
        printf("\n\t{\n" "\t\tcapacity\t = %lld\n" "\t\tsize\t\t = %lld\n" "\t\tdata[%lld]:[%p]\n" "\t\t{\n", s->data_capacity, s->data_size, s->data_capacity, s->data);
        for(long long i = 0; i < s->data_capacity; i ++) {
            printf("\t\t\t*[%lld]\t = %f\n", i, (float)*(s->data + i));
        }
        printf("\t\t}\n" "\t}\n");
    } else {
        printf("\n{\n\\ttcapacity\t = %lld\n\tsize\t\t = %lld\n\tdata[%lld]:[%p]\n}\n", s->data_capacity, s->data_size, s->data_capacity, s->data);
    }
}
