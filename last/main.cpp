#include "List.h"

void ListTest() {
    List list;
    ListCtor(&list);
    ListDumb(&list);

    t_data Data[8] = {-1, -2, -3, -4, -5, -6, -7, -8};

    ListPushBack(&list, Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, Data + 1);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, Data + 2);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, Data + 3);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, Data + 4);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, Data + 5);
    ListIsOk(&list);
    ListDumb(&list);

    ListDtor(&list);
}

int main(int argc, char* argv[]) {
    ListTest();

    return 0;
}
