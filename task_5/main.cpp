
#include "HashTable.h"

void ListTest() {
    List list;
    ListCtor(&list);
    ListDumb(&list);

    t_data Data = -10;

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListInsert(&list, list.head, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListInsert(&list, list.tail, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListRemove(&list, list.head);
    ListIsOk(&list);
    ListDumb(&list);

    ListRemove(&list, list.tail);
    ListIsOk(&list);
    ListDumb(&list);

    ListRemove(&list, list.head);
    ListIsOk(&list);
    ListDumb(&list);

    ListRemove(&list, list.tail);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListPushBack(&list, &Data);
    ListIsOk(&list);
    ListDumb(&list);

    ListDtor(&list);
}

int Hash_0(char* s) {
    return s[0];
}

int Hash_1(char* s) {
    return strlen(s);
}

int Hash_2(char* s) {
    int len = strlen(s);
    int sum = 0;

    for(int i = 0; i < len; i ++) {
        sum += s[i];
    }

    return sum;
}

void show_bits(char a) {
    char size = 8*sizeof(char);
    char mask = 1 << size - 1;
    for(int i = 0; i < size; i ++ ) {
        printf("%d", a & mask ? 1 : 0);
        a <<= 1;
    }
    printf("\n");
}

char rol(char dig) {
    int size = 8*sizeof(char);
    char mask = 1 << size - 1; // 10000000

    //show_bits(dig);

    char elder = mask & dig;

    //show_bits(elder);

    for(int i = 0; i < size - 1; i ++) {
        elder >>= 1;
        elder &= ~mask;
    }

    //show_bits(elder);

    dig <<= 1;
    dig |= elder;

    //show_bits(dig);

    //std::cout << std::endl;

    return dig;
}

int Hash_3(char* s) {
    int len = strlen(s);
    char H = 0;

    for(int i = 1; i < len; i ++) {
        char H_i = rol(s[i]);
        //show_bits(s[i]);
        //show_bits(H_i);
        //std::cout << std::endl;
        //show_bits(H);
        //show_bits(H_i);
        //std::cout << std::endl;
        H ^= H_i;
        //show_bits(H);

        //std::cout << std::endl;
        //std::cout << std::endl;
    }

    return (int)H + 255;
}

int Hash_4(char* s) {

}

int Hash(char* s, int flag) {
    switch(flag) {
        case 0: return Hash_0(s); break;
        case 1: return Hash_1(s); break;
        case 2: return Hash_2(s); break;
        case 3: return Hash_3(s); break;
        case 4: return Hash_4(s); break;
        default: assert(true); /* uncorrect flag */ break;
    }
}

int main(int argc, char* argv[]) {
    //ListTest();

    HashTable hta;

    HtaCtor(&hta);

    FILE *file = fopen("input.txt", "rt");

    assert(file);

    int count = 0;
    char string[100] = "";
    char* END_OF_FILE = "[END]";

    int t = 0;

    t_data Data[20000];

    do {
        t = fscanf(file, "%s", string);

        if(strcmp(END_OF_FILE, string) == 0)
            break;

        int length = strlen(string);

        bool is_continue = false;
        for(int i = 0; i < length; i ++) {
            if( ! ((string[i] >= 'a' && string[i] <= 'z') ||
                   (string[i] >= 'A' && string[i] <= 'Z') )) {
                is_continue = true;
                break;
            }
        }
        if(is_continue) continue;

        Data[count] = count;

        HtaPush(&hta, Hash(string, 3), Data + count);

        count ++;
    } while(t > 0);

    HtaDtor(&hta);

    return 0;
}
