#include "List.h"

const int N_LISTS = 500;

struct HashTable {
    List lists[N_LISTS];

    int n_lists;
};

void HtaCtor(HashTable*);
void HtaDtor(HashTable*);

int HtaPush(HashTable*, int, t_data*);
int HtaRemove(HashTable*, int, ptr);
