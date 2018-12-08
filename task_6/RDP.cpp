#include "Tree.h"
#include "RDP.h"

Calc :: Calc() {
    cur = NULL;

    msg = new short[MIN_MSG_SIZE];
    msg_max_size = MIN_MSG_SIZE;
    msg_size = 0;

    TreeCtor(&m_tree);
}

Calc :: ~Calc() {
    TreeDtor(&m_tree);

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

Node* Calc :: getG(char* pos) {
    assert(pos);
    cur = pos;

    Node *val = getE();

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

    m_tree.root = val;
    return val;
}

#define check_error if(!msg_is_emp()) {return 0;}
#define OR if(!msg_is_emp()) msg_pop(); else {return node;}

Node* Calc :: getE() {
    Node *node_1 = getT(); check_error

    while(*cur == '+' || *cur == '-') {
        char s = *cur;

        cur ++;

        Node* node_2 = getT(); check_error

        Node* node = TreeNodeCreate(&m_tree, Node::TYPE_OPR, s, node_1, node_2);
        node_1 = node;
    }

    return node_1;
}

Node* Calc :: getT() {
    Node *node_1 = getPo(); check_error

    while(*cur == '*' || *cur == '/') {
        char s = *cur;

        cur ++;

        Node* node_2 = getPo(); check_error

        Node* node = TreeNodeCreate(&m_tree, Node::TYPE_OPR, s, node_1, node_2);
        node_1 = node;
    }

    return node_1;
}

Node* Calc :: getPo() {
    Node *node_1 = getP(); check_error

    while(*cur == '^') {
        char s = *cur;

        cur ++;

        Node* node_2 = getP(); check_error

        Node* node = TreeNodeCreate(&m_tree, Node::TYPE_OPR, s, node_1, node_2);
        node_1 = node;
    }

    return node_1;
}

Node* Calc :: getP() {
    Node *node;

    if(*cur == '(') {
        cur ++;

        node = getE(); check_error

        if(*cur != ')') {
            msg_push(P_ERROR);
        } else {
            cur ++;
        }

        return node;
    }

    node = getN();
    OR node = getId();
    OR node = getFu();

    check_error

    return node;
}

Node* Calc :: getN() {
    Node* node = NULL;
    double val = 0;

    char* old_cur = cur;
    while(*cur >= '0' && *cur <= '9') {
        val = val * 10 + (double)(*cur - '0');
        cur ++;
    }

    if(old_cur == cur) {
        msg_push(N_ERROR);
    } else {
        std::cout << "DIG: " << (int)val << std::endl;
        node = TreeNodeCreate(&m_tree, Node::TYPE_DIG, (int)val, NULL, NULL);
    }

    return node;
}

Node* Calc :: getId() {
    Node* node = NULL;

    double val = 0;

    char* old_cur = cur;

    if(*cur == 'x') {
        val = *cur;
        cur ++;
    }

    if(old_cur == cur) {
        msg_push(ID_ERROR);
    } else {
        std::cout << "VAR: " << (char)val << std::endl;
        node = TreeNodeCreate(&m_tree, Node::TYPE_VAR, (int)val, NULL, NULL);
    }

    return node;
}

Node* Calc :: getFu() {
    Node* node = NULL;

    double val = 0;

    char* old_cur = cur;

    if(*cur == 's' || *cur == 'c' ||
       *cur == 'C' || *cur == 'S' ||
       *cur == 'a' || *cur == 'e' ||
       *cur == 'L') {
        val = *cur;
        cur ++;
    }

    if(old_cur == cur) {
        msg_push(FU_ERROR);
    } else {
        std::cout << "OPR: " << (char)val << std::endl;
        Node* node1 = getP(); check_error;

        node = TreeNodeCreate(&m_tree, Node::TYPE_OPR, (int)val, node1, NULL);
    }

    return node;
}
