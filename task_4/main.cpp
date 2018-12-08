/*!
    \file
    \brief Main file
    \author Yaroslav Pozndyak

    Main file with main()
*/

#include "Assembler.h"
#include "StringArray.h"
#include "Processor.h"
#include "Disassembler.h"
#include "Stack.h"


int main(int argc, char* argv[]) {

    Assembler ass = {};
    Disassembler das = {};
    Proc proc = {};

    const char prog_name[60] = "factram";
    char input_path[100] = "src\\";
    char das_path[100] = "das\\";
    char exe_path[100] = "exe\\";

    char _prog_name[60];
    strcpy(_prog_name, prog_name);
    strcat(input_path, strcat(_prog_name, ".pyas"));

    strcpy(_prog_name, prog_name);
    strcat(exe_path, strcat(_prog_name, ".bin"));

    strcpy(_prog_name, prog_name);
    strcat(das_path, strcat(_prog_name, ".txt"));

    printf("[OUT]\tpath to src '%s'\n", input_path);
    printf("[OUT]\tpath to exe '%s'\n", exe_path);
    printf("[OUT]\tpath to das '%s'\n", das_path);


    AssCtor(&ass);
    AssInput(&ass, input_path);
    AssCompile(&ass);
    AssOutput(&ass, exe_path);

    DasCtor(&das);
    DasInput(&das, exe_path);
    DasDecompile(&das);
    DasOutput(&das, das_path);

    ProcCtor(&proc);
    ProcInput(&proc, exe_path);

    ProcRun(&proc);

    AssDtor(&ass);
    DasDtor(&das);

    ProcDtor(&proc);
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
