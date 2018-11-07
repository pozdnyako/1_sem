#define PUSH(a) ProcPush(proc, a)
#define POP ProcPop(proc)

#define nxt_C proc->cp ++;
#define CODE proc->code[proc->cp]

#define USE_ARG(arg, type)\
        {\
            arg = *(type*)(proc->code + proc->cp);\
            proc->cp += sizeof(type) - 1;\
        }\

#define REG(num) proc->registers[num]

#define ARG_1 words[1]
#define ARG_2 words[2]
#define ARG_3 words[3]
#define ARG_4 words[4]

#define WRITE_ARG(arg, type) \
        memcpy(ass->bytecode + current_pos, (char*)&arg, sizeof(type));\
        current_pos += sizeof(type);\

/// MAX length of name is 10
/// MIN length of name is 3
/// MAX number of args is 4

DEF_CMD(PUSH, 1,
        {
            nxt_C
            stack_e arg = 0;
            USE_ARG(arg, double);
            PUSH(arg);
            nxt_C
        }
        , 1, push,
        {
            double pushed_num;
            sscanf(ARG_1, "%lf", &pushed_num);
            WRITE_ARG(pushed_num, double);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(POP, 2,
        {
            POP;
            nxt_C
        }
        , 0, pop,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(ADD, 3,
        {
            PUSH((double)POP + (double)POP);
            nxt_C
        }
        , 0, add,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(SUB, 4,
        {
            PUSH((double)POP-(double)POP);
            nxt_C
        }
        , 0, sub,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(MUL, 5,
        {
            PUSH((double)POP * (double)POP);
            nxt_C
        }
        , 0, mul,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(DIV, 6,
        {
            PUSH((double)POP / (double)POP);
            nxt_C
        }
        , 0, div,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(SIN, 7,
        {
            PUSH((stack_e)sin((double)POP));
            nxt_C
        }
        , 0, sin,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(SQRT, 8,
        {
            PUSH((stack_e)sqrt((double)POP));
            nxt_C
        }
        , 0, sqrt,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(IN, 9,
        {
            double a;
            scanf("%Lf", &a);
            PUSH((stack_e)a);
            nxt_C
        }
        , 0, in,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(OUT, 10,
        {
            printf("%f\n", (float)POP);
            nxt_C
        }
        , 0, out,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(PUSH_REG, 11,
        {
            nxt_C;
            char reg_n;
            USE_ARG(reg_n, char);

            switch(reg_n) {
            case CMD_REG_RAX : proc->registers[CMD_REG_RAX] = ProcPop(proc); break;
            case CMD_REG_RAY : proc->registers[CMD_REG_RAY] = ProcPop(proc); break;
            case CMD_REG_RAZ : proc->registers[CMD_REG_RAZ] = ProcPop(proc); break;
            case CMD_REG_RAA : proc->registers[CMD_REG_RAA] = ProcPop(proc); break;
            case CMD_REG_RAB : proc->registers[CMD_REG_RAB] = ProcPop(proc); break;
            default:
                ProcDump(proc, "error!");
                printf("[ERROR] register number %d wasn't declared", proc->code[proc->cp]);
                break;
            }

            /*#define DEF_REG(name, num) \
                case CMD_REG_##name : REG(num) = POP; break;
            switch(CODE) {
                #include "registers.h"
            default:
                printf("[ERROR] register number %d wasn't declared", CODE);
                break;
            }
            #undef DEF_REG
            */
            nxt_C
        }
        , 1, rpush,
        {
            char* reg_name = new char[3];
            char reg_num = -1;

            sscanf(ARG_1, "%s", reg_name);

            if(strcmp("RAX", reg_name) == 0) reg_num = CMD_REG_RAX;
            if(strcmp("RAY", reg_name) == 0) reg_num = CMD_REG_RAY;
            if(strcmp("RAZ", reg_name) == 0) reg_num = CMD_REG_RAZ;
            if(strcmp("RAA", reg_name) == 0) reg_num = CMD_REG_RAA;
            if(strcmp("RAB", reg_name) == 0) reg_num = CMD_REG_RAB;

            WRITE_ARG(reg_num, char);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(_JMP, 12,
        {
            nxt_C;
            long long arg = 0;
            USE_ARG(arg, long long);
            proc->cp = arg;
        }
        , 1, _jmp,
        {
            long long pos;
            sscanf(ARG_1, "%lld", &pos);
            WRITE_ARG(pos, long long);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(LBL, 13,
        {}
        , 1, lbl,
        {
            char *lbl_name = new char[10];

            sscanf(ARG_1, "%s", lbl_name);

            bool doesnt_find_label = true;
            for(int _i = 0; _i < ass->lbl_num; _i ++) {
                if(strcmp(ass->lbls[_i].name, lbl_name) == 0 && strlen(lbl_name) == strlen(ass->lbls[_i].name)) {
                    doesnt_find_label = false;
                }
            }

            if(doesnt_find_label && ass->lbl_num == MAX_NUM_LABELS) {
                printf("[ERROR] labels overflow");
                return 0;
            }

            if(doesnt_find_label) {
                ass->lbls[ass->lbl_num].name = lbl_name;
                ass->lbls[ass->lbl_num].position = current_pos;
                ass->lbl_num ++;

                printf("[COMPILE] new %d label with name %20s\tat pos\t%Ld \n", ass->lbl_num - 1, lbl_name, current_pos);
            } else if(ass->n_comp == 0){
                printf("[WARNING] redefintion of label %s", lbl_name);
            }
        }
        , NO_BYTECODE
        )
DEF_CMD(JMP, 14,
        {}
        , 1, jmp,
        {
            char *lbl_name = new char[10];

            sscanf(ARG_1, "%s", lbl_name);

            bool doesnt_find_label = true;
            long long m_lbl_pos = -1;
            for(int _i = 0; _i < ass->lbl_num; _i ++) {
                if(strcmp(ass->lbls[_i].name, lbl_name) == 0){
                    doesnt_find_label = false;
                    m_lbl_pos = ass->lbls[_i].position;
                }
            }

            if(!doesnt_find_label) {
                WRITE_ARG(m_lbl_pos, long long);
            } else if(ass->n_comp == 1) {
                printf("[ERROR] label %s doesnt find\n", lbl_name);
                return 0;
            } else {
                WRITE_ARG(m_lbl_pos, long long);
            }
        }
        , CMD__JMP
        )
DEF_CMD(POP_REG, 15,
        {
            nxt_C;
            char reg_n;
            USE_ARG(reg_n, char);

            switch(reg_n) {
            case CMD_REG_RAX : PUSH(REG(CMD_REG_RAX)); break;
            case CMD_REG_RAY : PUSH(REG(CMD_REG_RAY)); break;
            case CMD_REG_RAZ : PUSH(REG(CMD_REG_RAZ)); break;
            case CMD_REG_RAA : PUSH(REG(CMD_REG_RAA)); break;
            case CMD_REG_RAB : PUSH(REG(CMD_REG_RAB)); break;
            default:
                ProcDump(proc, "error!");
                printf("[ERROR] register number %d wasn't declared", proc->code[proc->cp]);
                return 0;
                break;
            }

            /*#define DEF_REG(name, num) \
                case CMD_REG_##name : REG(num) = POP; break;
            switch(CODE) {
                #include "registers.h"
            default:
                printf("[ERROR] register number %d wasn't declared", CODE);
                break;
            }
            #undef DEF_REG
            */
            nxt_C
        }
        , 1, rpop,
        {
            char* reg_name = new char[3];
            char reg_num = -1;

            sscanf(ARG_1, "%s", reg_name);

            if(strcmp("RAX", reg_name) == 0) reg_num = CMD_REG_RAX;
            if(strcmp("RAY", reg_name) == 0) reg_num = CMD_REG_RAY;
            if(strcmp("RAZ", reg_name) == 0) reg_num = CMD_REG_RAZ;
            if(strcmp("RAA", reg_name) == 0) reg_num = CMD_REG_RAA;
            if(strcmp("RAB", reg_name) == 0) reg_num = CMD_REG_RAB;

            WRITE_ARG(reg_num, char);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(_JA, 16,
        {
            nxt_C
            long long arg = 0;
            USE_ARG(arg, long long);
            if(POP > POP) {
                proc->cp = arg;
            } else {
                nxt_C
            }
        }
        , 1, _ja,
        {
            long long pos;
            sscanf(ARG_1, "%lld", &pos);
            WRITE_ARG(pos, long long);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(JA, 17,
        {}
        , 1, ja,
        {
            char *lbl_name = new char[10];

            sscanf(ARG_1, "%s", lbl_name);

            bool doesnt_find_label = true;
            long long m_lbl_pos = -1;
            for(int _i = 0; _i < ass->lbl_num; _i ++) {
                if(strcmp(ass->lbls[_i].name, lbl_name) == 0 && strlen(lbl_name) == strlen(ass->lbls[_i].name)){
                    doesnt_find_label = false;
                    m_lbl_pos = ass->lbls[_i].position;
                }
            }

            if(!doesnt_find_label) {
                WRITE_ARG(m_lbl_pos, long long);
            } else if(ass->n_comp == 1) {
                printf("[ERROR] label %s doesnt find\n", lbl_name);
                return 0;
            } else {
                WRITE_ARG(m_lbl_pos, long long);
            }
        }
        , CMD__JA
        )
DEF_CMD(_JE, 18,
        {
            nxt_C
            long long arg = 0;
            USE_ARG(arg, long long);

            if(POP == POP) {
                proc->cp = arg;
            } else {
                nxt_C
            }
        }
        , 1, _je,
        {
            long long pos;
            sscanf(ARG_1, "%lld", &pos);
            WRITE_ARG(pos, long long);
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(JE, 19,
        {}
        , 1, je,
        {
            char *lbl_name = new char[10];

            sscanf(ARG_1, "%s", lbl_name);

            bool doesnt_find_label = true;
            long long m_lbl_pos = -1;
            for(int _i = 0; _i < ass->lbl_num; _i ++) {
                if(strcmp(ass->lbls[_i].name, lbl_name) == 0 && strlen(lbl_name) == strlen(ass->lbls[_i].name)){
                    doesnt_find_label = false;
                    m_lbl_pos = ass->lbls[_i].position;
                }
            }

            if(!doesnt_find_label) {
                WRITE_ARG(m_lbl_pos, long long);
            } else if(ass->n_comp == 1) {
                printf("[ERROR] label %s doesnt find\n", lbl_name);
                return 0;
            } else {
                WRITE_ARG(m_lbl_pos, long long);
            }
        }
        , CMD__JE
        )
DEF_CMD(CALL, 20,
        {
            nxt_C;
            long long arg = 0;
            USE_ARG(arg, long long);

            StackPush(proc->calls, proc->cp);
            proc->cp = arg;
        }
        , 1, call,
        {
            char *lbl_name = new char[10];

            sscanf(ARG_1, "%s", lbl_name);

            bool doesnt_find_label = true;
            long long m_lbl_pos = -1;
            for(int _i = 0; _i < ass->lbl_num; _i ++) {
                if(strcmp(ass->lbls[_i].name, lbl_name) == 0 && strlen(lbl_name) == strlen(ass->lbls[_i].name)){
                    doesnt_find_label = false;
                    m_lbl_pos = ass->lbls[_i].position;
                }
            }

            if(!doesnt_find_label) {
                WRITE_ARG(m_lbl_pos, long long);
            } else if(ass->n_comp == 1) {
                printf("[ERROR] label %s doesnt find\n", lbl_name);
                return 0;
            } else {
                WRITE_ARG(m_lbl_pos, long long);
            }

            calls[call_n] = current_pos;
            call_n ++;
        }
        , NORMAL_BYTECODE
        )
DEF_CMD(RET, 21,
        {
            long long pos = StackPop(proc->calls);
            proc->cp = pos;
        }
        , 0, ret,
        {}
        , NORMAL_BYTECODE
        )
DEF_CMD(END, 0,
        {
            ;
        }
        , 0, end,
        {}
        , NORMAL_BYTECODE
        )
#undef nxt_C
#undef CODE
#undef REG
#undef ARG_1
#undef ARG_2
#undef ARG_3
#undef ARG_4
