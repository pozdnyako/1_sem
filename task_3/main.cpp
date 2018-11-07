/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    Main file with main()
*/

#include "Stack.h"

int main(int argc, char* argv[]) {
    Stack s = {};
    int s_cap = 3;


    StackCtor(&s, s_cap);
    UNIT_TEST(s.data_capacity, s_cap);


    int s_0 = 100, s_1 = 101, s_2 = 102, s_3 = 103;
    StackPush(&s, s_0);
    UNIT_TEST(s.data[0], s_0);

    StackPush(&s, s_1);
    UNIT_TEST(s.data[1], s_1);

    StackPush(&s, s_2);
    UNIT_TEST(s.data[2], s_2);

    UNIT_TEST(StackPush(&s, s_3), Stack::DATA_ERROR);

    UNIT_TEST(StackPop(&s), s_2);
    UNIT_TEST(StackPop(&s), s_1);
    UNIT_TEST(StackPop(&s), s_0);

    UNIT_TEST(StackPop(&s), Stack::DATA_ERROR);


    StackDtor(&s);

    UNIT_TEST(s.data, NULL);
    UNIT_TEST(s.data_capacity, Stack::DATA_ERROR);
    UNIT_TEST(s.data_size, 0);
    return 0;
}
