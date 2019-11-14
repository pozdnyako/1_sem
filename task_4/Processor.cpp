#include "Processor.h"
#include "Stack.h"

void ProcCtor(Proc* proc) {
    assert(proc);
    assert(!proc->m_stack);
    assert(!proc->calls);
    assert(!proc->ram_use);
    assert(!proc->code);
    assert(!proc->ram);

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

    proc->ram_use = new Stack;
    *(proc->ram_use) = {};

    proc->ram = new stack_e[Proc::RAM_SIZE];

    for(int i = 0; i < Proc::RAM_SIZE; i ++) {
        proc->ram[i] = 0;
    }

    StackCtor(proc->m_stack, Proc::STACK_SIZE);
    StackCtor(proc->calls, Proc::MAX_CALL_NUM);
    StackCtor(proc->ram_use, Proc::MAX_CALL_NUM+1);
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
    StackDump(proc->calls, "calls");
    StackDump(proc->ram_use, "ram_use");

    printf("\tRAM : {");
    for(int i = 0; i < Proc::RAM_SIZE; i ++) {
        printf("%d\t", proc->ram[i]);
    }
    printf("}\n");

    printf("}\n");
}

int ProcRun(Proc *proc) {
    assert(proc);

    RAM_REGISTER_CUR = 0; /// RAX
    RAM_REGISTER_MAX = 0; /// RAY

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

    for(int i = 0; i < 10; i ++) {
        if(RAM_REGISTER_CUR == i) {
            printf("->");
        }
        if(RAM_REGISTER_MAX == i) {
            printf("|");
        }

        printf("%.0f ", proc->ram[i]);
    }
    printf("\n");
    for(int i = 0; i < 10; i ++) {
        if(proc->m_stack->data_size == i) {
            printf("|");
        }

        printf("%.0f ", proc->m_stack->data[i]);
    }
    printf("\n");

    for(int i = 0; i < 10; i ++) {
        if(proc->calls->data_size == i) {
            printf("|");
        }

        printf("%.0f ", proc->calls->data[i]);
    }
    printf("\n\n");

    #undef DEF_CMD
}
