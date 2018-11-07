#include "info.h"

struct Proc {
    Stack *m_stack;
    Stack *calls;

    long long code_size;
    char *code;
    long long cp;

    long long tick_n;
    stack_e* registers;

    static const long long STACK_SIZE = 10;
    static const long long MAX_CALL_NUM = 100;
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
