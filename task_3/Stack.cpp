#include "Stack.h"

void StackCtor(Stack *s, long long data_capacity) {
    assert(s);

    s->data = (int*)calloc(data_capacity, sizeof(int));
    s->data_capacity = data_capacity;
    s->data_size = 0;
}

void StackDtor(Stack *s) {
    assert(s);

    free(s->data);

    s->data = NULL;
    s->data_capacity = Stack::DATA_ERROR;
    s->data_size = 0;
}

int StackPush(Stack *s, int val) {
    ASSERT_OK(*s);

    if(!(s->data_size + 1 <= s->data_capacity)) {
        return Stack::DATA_ERROR;
    }
    *(s->data + s->data_size) = val;
    s->data_size ++;

    ASSERT_OK(*s);
}

int StackPop(Stack* s) {
    ASSERT_OK(*s);

    if(!(s->data_size > 0)) {
        return Stack::DATA_ERROR;
    }

    s->data_size --;

    int val = *(s->data + s->data_size);
    *(s->data + s->data_size) = 0;

    ASSERT_OK(*s);

    return val;
}

bool StackOk(Stack *s) {
    return s && s->data && (s->data_size >= 0 && s->data_size <= s->data_capacity) && (s->data_capacity != Stack::DATA_ERROR);
}

void StackDumb(Stack *s, const char* nm) {
    printf("Stack %s[%p] ", nm, s);

    if(StackOk(s))
        printf("(OK)");
    else
        printf("(ERROR)");

    if(s->data) {
        printf("\n{\n\tcapacity\t = %lld\n\tsize\t\t = %lld\n\tdata[%lld]:[%p]\n\t{\n", s->data_capacity, s->data_size, s->data_capacity, s->data);
        for(long long i = 0; i < s->data_capacity; i ++) {
            printf("\t\t*[%lld]\t = %d \n", i, *(s->data + i));
        }
        printf("\t}\n}\n");
    } else {
        printf("\n{\n\tcapacity\t = %lld\n\tsize\t\t = %lld\n\tdata[%lld]:[%p]\n}\n", s->data_capacity, s->data_size, s->data_capacity, s->data);
    }
}
