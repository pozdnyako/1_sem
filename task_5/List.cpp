#include "List.h"

void ElmCtor(Element* elm) {
    assert(elm);

    elm->next = N_PTR;
    elm->prev = N_PTR;

    elm->data = NULL;
}

void ListCtor(List* lst) {
    assert(lst);

    lst->length = 0;
    lst->is_sorted = true;

    lst->max_length = MIN_LIST_LENGTH;

    lst->elm = new Element[lst->max_length + 1];

    for(int i = 0; i < lst->max_length + 1; i ++) {
        ElmCtor(lst->elm + i);

        if(i == 0)
            continue;

        if(i < lst->max_length)
            (lst->elm + i)->next = i + 1;
        else
            (lst->elm + i)->next = LIST_END;
    }

    lst->head = LIST_CLR;
    lst->tail = LIST_CLR;
    lst->free = 1;
}

void ListDtor(List *lst) {

}

void ListMemx2(List *lst) {
    int len = lst->max_length;

    lst->max_length *= 2;
    lst->elm = (Element*)realloc(lst->elm, sizeof(Element) * (lst->max_length + 1) );

    for(int i = len; i < lst->max_length + 1; i ++) {
        if(i != len)
            ElmCtor(lst->elm + i);

        if(i < lst->max_length)
            (lst->elm + i)->next = i + 1;
        else
            (lst->elm + i)->next = LIST_END;
    }
}

ptr ListInsert(List* lst, const ptr _elm, t_data *_data) {
    assert(lst);

    assert(_elm >= 1); assert(_elm < lst->max_length + 1);

    if(lst->length + 1 == lst->max_length) {
        ListMemx2(lst);
    }

    if(lst->elm[_elm].prev == N_PTR) {
        printf("[ERROR] unknow element" "\n");
        return -1;
    }

    ptr cur = lst->free;
    lst->free = lst->elm[cur].next;

    lst->elm[cur].next = _elm;
    lst->elm[cur].prev = lst->elm[_elm].prev;

    if(lst->elm[_elm].prev == LIST_TAIL) {
        lst->tail = cur;

    } else {
        lst->elm[lst->elm[_elm].prev].next = cur;
    }

    lst->elm[_elm].prev = cur;

    lst->length ++;

    if(lst->is_sorted && cur != _elm - 1) {
        lst->is_sorted = false;
    }

    lst->elm[cur].data = _data;

    return cur;
}

ptr ListPushBack(List* lst, t_data *_data) {
    assert(lst);

    if(lst->length + 1 == lst->max_length) {
        ListMemx2(lst);
    }

    ptr cur = lst->free;
    lst->free = lst->elm[cur].next;
    lst->elm[cur].next = LIST_HEAD;

    if(lst->length == 0) {
        lst->elm[cur].prev = LIST_TAIL;
        lst->tail = cur;
    } else {
        lst->elm[cur].prev = lst->head;
        lst->elm[lst->head].next = cur;
    }

    if(lst->is_sorted && cur != lst->head + 1 && lst->length != 0) {
        lst->is_sorted = false;
    }

    lst->head = cur;

    lst->length ++;

    lst->elm[cur].data = _data;

    return cur;
}

ptr ListRemove(List* lst, const ptr _elm) {
    assert(lst);

    assert(_elm >= 1); assert(_elm < lst->max_length + 1);

    if(lst->elm[_elm].prev == N_PTR) {
        printf("[ERROR] unknow element" "\n");
        return -1;
    }

    lst->is_sorted = false;

    bool is_tail = false,
         is_head = false;

    is_tail = (lst->elm[_elm].prev == LIST_TAIL);
    is_head = (lst->elm[_elm].next == LIST_HEAD);

    if(is_tail && is_head) {
       lst->tail = LIST_CLR;
       lst->head = LIST_CLR;
    } else {

        if(!is_tail) {
            lst->elm[lst->elm[_elm].prev].next = lst->elm[_elm].next;

        } else {
            lst->tail = lst->elm[_elm].next;
        }

        if(!is_head) {
            lst->elm[lst->elm[_elm].next].prev = lst->elm[_elm].prev;

        } else {
            lst->head = lst->elm[_elm].prev;
        }
    }

    lst->elm[_elm].prev = N_PTR;
    lst->elm[_elm].next = lst->free;

    lst->length --;

    lst->free = _elm;

    return 1;
}


void ListDumb(List* lst) {
    assert(lst);

    printf("List dumb" "\n" "{" "\n");
    printf("\tmax length: %d\n\tlength: %d\n", lst->max_length, lst->length);

    printf("\tElements:\n");

    for(int i = 0; i < lst->max_length + 1; i ++) {
        printf("\t" "%4d ", i);
    }   printf("\tptr\n");

    for(int i = 0; i < lst->max_length + 1; i ++) {
        if(lst->elm[i].data != NULL) {
            printf("\t" "%4d ", (int)*(lst->elm[i].data));
        } else {
            printf("\t" "%4d ", 0);
        }
    }       printf("\t"                    "data" "\n");

    for(int i = 0; i < lst->max_length + 1; i ++) {
        printf("\t" "%4d ", lst->elm[i].next);
    }   printf("\t"                    "next" "\n");

    for(int i = 0; i < lst->max_length + 1; i ++) {
        printf("\t" "%4d ", lst->elm[i].prev);
    }   printf("\t"                    "prev" "\n");

    printf("\n");
    printf("\t" "%4d" "\t" "head" "\n",
                       lst->head);

    printf("\t" "%4d" "\t" "tail" "\n",
                       lst->tail);

    printf("\t" "%4d" "\t" "free" "\n",
                       lst->free);

    printf("\t" "%4d" "\t" "length" "\n",
                       lst->length);
    printf("\t" "%4d" "\t" "is sorted" "\n",
                       lst->is_sorted);
    printf("}\n\n");

}

int ListIsOk(List* lst) {
    assert(lst);

    if(lst->length == 0) {
        if(!(lst->head == LIST_CLR &&
             lst->tail == LIST_CLR)) {
            printf("[ERROR] wrong 'head' or 'last' when length = 0\n");                 return -1;
        }
    } else {
        if(!(lst->elm[lst->head].next == LIST_HEAD &&
             lst->elm[lst->tail].prev == LIST_TAIL)) {
            printf("[ERROR] wrong 'head' or 'last' when length != 0\n");                return -2;
        }
        ptr cp = lst->tail;
        int last = LIST_TAIL;

        for(int i = 0; i < lst->length; i ++) {
            if(lst->elm[cp].prev != last) {
                printf("[ERROR] next and prev are not ñoordinated elment %d\n", cp);    return -3;
            }

            last = cp;
            cp = lst->elm[cp].next;
        }

        if(cp != LIST_HEAD) {
            printf("[ERROR] list doesn't finished by head\n");                          return -4;
        }
    }
    if(lst->length != lst->max_length) {
        ptr cp = lst->free;

        for(int i = 0; i < lst->max_length - lst->length; i ++) {
            cp = lst->elm[cp].next;
        }

        if(cp != LIST_END){
            printf("[ERROR] wrong next in clear elements\n");                           return -5;
        }
    }
}

int ListFind(List *lst, int num) {
    assert(lst);

    if(num >= lst->length) {
        return DONT_FIND;
    } if(num == lst->length - 1) {
        return lst->head;
    }

    ptr cp = lst->tail;

    for(int i = 0; i < num; i ++) {
        cp = lst->elm[cp].next;
    }

    return cp;
}
