/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    Main file with main()
*/

#include "Assembler.h"
#include "StringArray.h"
#include "Processor.h"
#include "Stack.h"


int main(int argc, char* argv[]) {

    Assembler ass = {};
    Proc proc = {};

    char prog_name[60] = "fib_";
    char input_path[100] = "src/";
    char exe_path[100] = "exe";

    strcat(input_path, strcat(prog_name, ".pyas"));
    strcat(exe_path, strcat(prog_name, ".bin"));

    AssCtor(&ass);
    AssInput(&ass, input_path);
    AssCompile(&ass);
    AssOutput(&ass, exe_path);

    ProcCtor(&proc);
    ProcInput(&proc, exe_path);

    ProcRun(&proc);
    return 0;
}

/*
push 1
pop ax

...
...

push ax
call 5               -|
// next_command <-----|--------
                      |       |
...                   |       |
...                   |       |
                      |       |
:5  <------------------       |
    pop bx                    |
    push bx                   |
    push bx                   |
    mul                       |
    ret ----------------------|
*/
