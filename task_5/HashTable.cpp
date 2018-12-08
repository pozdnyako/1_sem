#include "HashTable.h"

void HtaCtor(HashTable *hta) {
    assert(hta);

    for(int i = 0; i < N_LISTS; i ++) {
        ListCtor(hta->lists + i);
    }
}

void HtaDtor(HashTable *hta) {
    assert(hta);

    for(int i = 0; i < N_LISTS; i ++) {
        ListDtor(hta->lists + i);
    }
}

int HtaPush(HashTable *hta, int c_list, t_data *_data) {
    assert(hta);
    assert(_data);

    if(c_list >= hta->n_lists)
        hta->n_lists = c_list;

    ListPushBack(hta->lists + c_list, _data);

    return 1;
}

int HtaRemove(HashTable *hta, int c_list, ptr _ptr) {
    assert(hta);

    if(c_list >= hta->n_lists) {
        printf("[ERROR] don't have %d list, cause n_lists is %d\n", c_list, hta->n_lists);
        return -1;
    }

    ListRemove(hta->lists + c_list, _ptr);

    return 1;
}

void HtaDumb(HashTable *hta) {
    assert(hta);

    printf("HashTable: {\n");
    printf("\tn_lisits: %d", hta->n_lists);

    for(int i = 0; i < hta->n_lists; i ++) {
        ListDumb(hta->lists + i);
    }

    printf("}\n");
}
