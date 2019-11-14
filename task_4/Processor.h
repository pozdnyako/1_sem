#include "info.h"

#define RAM_REGISTER_CUR proc->registers[0] ///RAX
#define RAM_REGISTER_MAX proc->registers[1] ///RAY

struct Proc {
    Stack *m_stack;
    Stack *calls;
    Stack *ram_use;

    long long code_size;
    char *code;
    long long cp;

    long long tick_n;
    stack_e* registers;

    stack_e* ram;

    static const long long STACK_SIZE = 1000;
    static const long long MAX_CALL_NUM = 1000;
    static const long long RAM_SIZE = 1000;
};

void ProcCtor(Proc*);
void ProcDtor(Proc*);

int ProcInput(Proc*, const char*);

int ProcPush(Proc*, stack_e);
stack_e ProcPop(Proc*);

bool ProcOk(Proc*);
void ProcDump(Proc*, const char*);

int ProcRun(Proc*);
int ProcTick(Proc*);
