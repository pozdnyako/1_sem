#include "Processor.h"
#include "Stack.h"

void ProcCtor(Proc* proc) {
    assert(proc);
    assert(!proc->m_stack);
    assert(!proc->calls);
    assert(!proc->code);

    proc->code_size = 0;
    proc->cp = 0;
    proc->tick_n = 0;

    proc->registers = new stack_e[CMD_REG_MAX];
    #define DEF_REG(name, num) proc->registers[CMD_REG_##name] = 0;
    #include "registers.h"
    #undef DEF_REG

    proc->m_stack = new Stack;
    *(proc->m_stack) = {};

    proc->calls = new Stack;
    *(proc->calls) = {};

    StackCtor(proc->m_stack, Proc::STACK_SIZE);
    StackCtor(proc->calls, Proc::MAX_CALL_NUM);
}

void ProcDtor(Proc* proc) {
    assert(proc);
    assert(proc->m_stack);

    StackDtor(proc->m_stack);
    proc->m_stack = NULL;
}

int ProcInput(Proc *proc, const char* path) {
    assert(proc);

    size_t buf_size;
    scan_str_with_delete_and_new(path, &buf_size, &proc->code);
    proc->code_size = buf_size;
}

int ProcPush(Proc* proc, stack_e new_e) {
    assert(proc);
    StackPush(proc->m_stack, new_e);
}

stack_e ProcPop(Proc* proc) {
    assert(proc);
    return StackPop(proc->m_stack);
}

bool ProcOk(Proc* proc) {
    assert(proc);
    return StackOk(proc->m_stack);
}

void ProcDump(Proc* proc, const char* name) {
    assert(proc);

    printf("Processor %s {\n", name);

    printf("\tcode_size = %lld\n", proc->code_size);
    printf("\tcode : { ");
    for(long long i = 0; i < proc->code_size; i ++) {
        if(i == proc->cp) printf("->");
        printf("%hd ", proc->code[i]);
    }
    printf(" }\n");
    printf("\tcp = %lld\n", proc->cp);
    printf("\ttick_n = %lld\n", proc->tick_n);

    printf("\tregisters : {\n");
    #define DEF_REG(name, num) printf("\t\t%s(%d) = %f\n", #name, num, (float)proc->registers[num]);
    #include "registers.h"
    #undef DEF_REG
    printf("\t}\n");

    StackDump(proc->m_stack, "m_stack");
    printf("}\n");
}

int ProcRun(Proc *proc) {
    assert(proc);

    proc->tick_n = 0;
    printf("[OUT]\tprocessor starts\n");
    while(proc->code[proc->cp] != CMD_END) {
        ProcTick(proc);
    }
    printf("[OUT]\tprocessor finished succsessful\n");
}

int ProcTick(Proc* proc) {
    assert(proc);

    proc->tick_n ++;
    #define DEF_CMD(name,  num, code, ...) \
        case CMD_##name : code break;
    switch(proc->code[proc->cp]) {
        #include "commands.h"
    default:
        printf("[ERORR] function %hd in position %d wasn't declared", proc->code[proc->cp], proc->cp);
        break;
    }

    //ProcDump(proc, "me");
    #undef DEF_CMD
}
