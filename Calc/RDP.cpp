#include "RDP.h"

Calc :: Calc() {
    cur = NULL;

    msg = new short[MIN_MSG_SIZE];
    msg_max_size = MIN_MSG_SIZE;
    msg_size = 0;
}

Calc :: ~Calc() {


}

void Calc :: msg_push(short _msg) {
    if(msg_size >= msg_max_size) {
        msg_max_size *= 2;
        realloc(msg, sizeof(short) * msg_max_size);
    }

    msg[msg_size] = _msg;
    msg_size ++;
}

short Calc :: msg_front() {
    assert(msg_size > 0);

    return msg[msg_size];
}

short Calc :: msg_pop() {
    assert(msg_size > 0);

    short val = msg[msg_size];

    msg[msg_size] = 0;
    msg_size --;

    return val;
}

void Calc :: msg_clr() {
    delete[] msg;
}

void Calc :: msg_print() {
    for(int i = msg_size - 1; i >= 0; i --) {
        printf("%d ", msg[i]);
    }
}

bool Calc :: msg_is_emp() {
    return (msg_size == 0);
}

double Calc :: getG(char* pos) {
    assert(pos);
    cur = pos;

    double val = 0;

    val = getE();

    if(*cur != '\0') {
        msg_push(END_ERROR);
    } else {
        cur ++;
    }

    if(!msg_is_emp()) {
        printf("[ERROR] syntxis error in position %d: ", cur - pos);
        msg_print();
        printf("\n");
        msg_clr();
    }

    return val;
}

#define check_error if(!msg_is_emp()) {return 0;}

double Calc :: getE() {
    double val = 0;

    val = getT(); check_error

    while(*cur == '+' || *cur == '-') {
        char s = *cur;

        cur ++;

        double val2 = getT(); check_error

        switch(s) {
            case '+' :  val += val2; break;
            case '-' :  val -= val2; break;
        }
    }

    return val;
}

double Calc :: getT() {
    double val = 0;

    val = getP(); check_error

    while(*cur == '*' || *cur == '/') {
        char s = *cur;

        cur ++;

        double val2 = getP(); check_error

        switch(s) {
            case '*' :  val *= val2; break;
            case '/' :  val /= val2; break;
        }
    }

    return val;
}

double Calc :: getP() {
    double val = 0;

    if(*cur == '(') {
        cur ++;

        val = getE(); check_error

        if(*cur != ')') {
            msg_push(P_ERROR);
        } else {
            cur ++;
        }

        return val;
    }

    val = getN(); check_error
    return val;
}

double Calc :: getN() {
    double val = 0;

    char* old_cur = cur;
    while(*cur >= '0' && *cur <= '9') {
        val = val * 10 + (double)(*cur - '0');
        cur ++;
    }

    if(old_cur == cur) {
        msg_push(N_ERROR);
    }

    return val;
}
