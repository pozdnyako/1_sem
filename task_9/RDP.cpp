#include "token.h"
#include "Tree.h"
#include "RDP.h"

Parser :: Parser() {
    cur = NULL;

    msg = new short[MIN_MSG_SIZE];
    msg_max_size = MIN_MSG_SIZE;
    msg_size = 0;
}

Parser :: ~Parser() {
    TreeDtor(m_tree);

}

void Parser :: msg_push(short _msg) {
    if(msg_size >= msg_max_size) {
        msg_max_size *= 2;
        realloc(msg, sizeof(short) * msg_max_size);
    }

    msg[msg_size] = _msg;
    msg_size ++;
}

short Parser :: msg_front() {
    assert(msg_size > 0);

    return msg[msg_size];
}

short Parser :: msg_pop() {
    assert(msg_size > 0);

    short val = msg[msg_size];

    msg[msg_size] = 0;
    msg_size --;

    return val;
}

void Parser :: msg_clr() {
    delete[] msg;
}

void Parser :: msg_print() {
    for(int i = msg_size - 1; i >= 0; i --) {
        printf("%d ", msg[i]);
    }
}

bool Parser :: msg_is_emp() {
    return (msg_size == 0);
}

Node* Parser :: getG(Token* pos) {
    assert(pos);
    cur = pos;

    Node *val = getM();

    if(cur->type != Token::type_end) {
        msg_push(END_ERROR);
    } else {
        cur ++;
    }

    if(!msg_is_emp()) {
        printf("%s", cur->data);
        printf("[ERROR] syntxis error in position %d: ", cur - pos);
        msg_print();
        printf("\n");
        msg_clr();
    }

    m_tree->root = val;
    return val;
}

#define check_error if(!msg_is_emp()) {return 0;}
#define OR if(!msg_is_emp()) msg_pop(); else {return node;}

Node* Parser :: getM() {
    Node* node_1 = getDecFu();

    if(!node_1)
        node_1 = getDecVar();

    if(!node_1) {
        msg_push(A_ERROR);
        return NULL;
    }

    Node* node_2 = NULL;
    Node* cur_node = node_1;
    do{
        Token* _cur = cur;
        node_2 = getDecFu();

        if(node_2) {
           cur_node->right = node_2;
           node_2->par = cur_node;
           cur_node = node_2;
           continue;
        }

        cur = _cur;
        node_2 = getDecVar();

        if(node_2) {
           cur_node->right = node_2;
           node_2->par = cur_node;
           cur_node = node_2;
           continue;
        }

    }while(node_2 != NULL);

    return node_1;
}

Node* Parser :: getDecVar() {
    Node *node_1 = NULL; check_error

    if(cur->type == Token::type_keyword && cur->val == 2) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_DEC_VAR, "Dec var", 7, NULL, NULL);

        cur ++;
        if(cur->type == Token::type_id /* don't have this var */) {
            Node *node_2 = TreeNodeCreate(m_tree, Node::TYPE_VAR, cur->data, cur->data_len, NULL, NULL);
            node_1->left = node_2;
            node_2->par = node_1;

            cur ++;

            if(cur->type == Token::type_separator && cur->val == ';') {
                cur ++;
                return node_1;

            } else {
                return NULL;
            }
        } else {
            return NULL;
        }
    }

    return node_1;
}


Node* Parser :: getDecFu() {
    Node *node_1 = NULL; check_error

    if(cur->type == Token::type_keyword && cur->val == 3) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_DEC_FU, "Dec function", 14, NULL, NULL);
        cur ++;

        Node *node_2 = getFu();

        if(!node_2) {
            return NULL;
        }

        node_1->left = node_2;
        node_2->par = node_1;
    }

    return node_1;
}

Node* Parser :: getFu() {
    Node *node_1 = NULL;

    if(cur->type == Token::type_id /* don't have this fu */) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_FU, cur->data, cur->data_len, NULL, NULL);
    }

    if(!node_1) {
        return NULL;
    }
    cur ++;


    if(cur->type == Token::type_separator && cur->val == '(') {
        cur ++;
    } else {
        return NULL;
    }

    Node *node_2 = getFuArgs();

    if(!node_2) {
        return NULL;
    }

    if(cur->type == Token::type_separator && cur->val == ')') {
        cur ++;
    } else {
        return NULL;
    }

    node_1->left = node_2;
    node_2->par = node_1;


    if(cur->type == Token::type_separator && cur->val == '{') {
        cur ++;
    } else {
        return NULL;
    }

    Node* node_3 = getCode();

    if(!node_3) {
        return NULL;
    }

    node_1->right = node_3;
    node_3->par = node_1;

    if(cur->type == Token::type_separator && cur->val == '}') {
        cur ++;
    } else {
        return NULL;
    }

    return node_1;
}

Node* Parser :: getFuArgs() {
    Node *node_1 = TreeNodeCreate(m_tree, Node::TYPE_ARG, "arg", 3, NULL, NULL);
    Node* root = node_1;

    int n = 0;

    do{
        if(n > 0) {
            if(cur->type == Token::type_separator && cur->val == ','){
                cur ++;
            } else {
                break;
            }
        }

        Node* node_2 = getFuArg();

        if(!node_2) {
            if(n == 0)
                break;
            else {
                return NULL;
            }
        }

        Node* node_3 = TreeNodeCreate(m_tree, Node::TYPE_ARG, "arg", 3, NULL, NULL);

        node_1->right = node_3;
        node_1->left = node_2;

        node_3->par = node_1;
        node_2->par = node_1;

        node_1 = node_3;
        n++;
    } while(true);

    return root;
}

Node* Parser :: getFuArg() {
    Node *node_1 = NULL; check_error

    if(cur->type == Token::type_keyword && cur->val == 2) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_DEC_ARG, "Dec arg", 7, NULL, NULL);

        cur ++;
        if(cur->type == Token::type_id /* don't have this var */) {
            Node *node_2 = TreeNodeCreate(m_tree, Node::TYPE_VAR, cur->data, cur->data_len, NULL, NULL);
            node_1->left = node_2;
            node_2->par = node_1;

            cur ++;
        } else {
            return NULL;
        }
    }

    return node_1;
}

Node* Parser :: getCode() {
    Node *node_1 = TreeNodeCreate(m_tree, Node::TYPE_CODE, "Code", 4, NULL, NULL);
    Node* root = node_1;

    while(true) {
        Token *old = cur;

        Node* node_2 = NULL;

        if(!node_2) {
            cur = old;
            node_2 = getIf();
        }

        if(!node_2) {
            cur = old;
            node_2 = getWh();
        }

        if(!node_2) {
            cur = old;
            node_2 = getSca();
        }

        if(!node_2) {
            cur = old;
            node_2 = getPri();
        }

        if(!node_2) {
            cur = old;
            node_2 = getSqrt();
        }

        if(!node_2) {
            cur = old;
            node_2 = getRet();
        }

        if(!node_2) {
            cur = old;
            node_2 = getDecVar();
        }

        if(!node_2) {
            cur = old;
            node_2 = getFreeCall();
        }

        if(!node_2) {
            cur = old;
            node_2 = getAss();
        }

        if(!node_2) {
            break;
        }

        Node* node_3 = TreeNodeCreate(m_tree, Node::TYPE_CODE, "Code", 4, NULL, NULL);

        node_1->left = node_2;
        node_1->right = node_3;

        node_2->par = node_1;
        node_3->par = node_1;

        node_1 = node_3;
    }

    return root;
}

Node* Parser :: getVar() {
    Node* node = NULL;
    if(cur->type == Token::type_id /* have this var */) {
        node = TreeNodeCreate(m_tree, Node::TYPE_VAR, cur->data, cur->data_len, NULL, NULL);
        cur ++;
    } else {
        return NULL;
    }

    return node;
}

Node* Parser :: getAss() {
    Node *node_2 = getVar();
    if(!node_2) {
        return NULL;
    }

    if(cur->type == Token::type_operator && cur->val == '=') {
        cur ++;
    } else {
        return NULL;
    }

    Node *node_1 = TreeNodeCreate(m_tree, Node::TYPE_ASS, "=", 1, NULL, NULL);

    Node *node_3 = getE();

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }


    node_1->left = node_2;
    node_1->right = node_3;

    node_2->par = node_1;
    node_3->par = node_1;

    return node_1;
}

Node* Parser :: getE() {
    Node *node_1 = getOr(); check_error

    if(cur->type == Token::type_operator && cur->val == '~'){
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getOr(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }

    return node_1;
}

Node* Parser :: getOr() {
    Node *node_1 = getAnd(); check_error

    while(cur->type == Token::type_operator && cur->val == '&'){
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getAnd(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }

    return node_1;
}

Node* Parser :: getAnd() {
    Node *node_1 = getOrd(); check_error

    while(cur->type == Token::type_operator && cur->val == '&'){
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getOrd(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }

    return node_1;
}

Node* Parser :: getOrd() {
    Node *node_1 = getS(); check_error

    if((cur->type == Token::type_operator && cur->val == '>') ||
       (cur->type == Token::type_operator && cur->val == '<') ||
       (cur->type == Token::type_operator && cur->val == '=')) {
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getS(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }

    return node_1;
}

Node* Parser :: getS() {
    Node *node_1 = getT(); check_error

    while((cur->type == Token::type_operator && cur->val == '+') ||
          (cur->type == Token::type_operator && cur->val == '-')) {
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getT(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }

    return node_1;
}

Node* Parser :: getT() {
    Node *node_1 = getP(); check_error

    while((cur->type == Token::type_operator && cur->val == '*') ||
          (cur->type == Token::type_operator && cur->val == '/')) {
        char s[1] = {cur->val};

        cur ++;

        Node* node_2 = getP(); check_error

        Node* node = TreeNodeCreate(m_tree, Node::TYPE_OPR, s, 1, node_1, node_2);
        node_1->par = node;
        node_2->par = node;

        node_1 = node;
    }
    return node_1;
}

Node* Parser :: getP() {
    Node *node;
    if(cur->type == Token::type_separator && cur->val == '(') {
        cur ++;

        node = getE(); check_error

        if(!(cur->type == Token::type_separator && cur->val == ')')) {
            msg_push(P_ERROR);
        } else {
            cur ++;
        }

        return node;
    }

    Token *old = cur;
    node = getLet();

    if(node == NULL) {
        cur = old;
        node = getCall();

    if(node == NULL) {
        cur = old;
        node = getVar();

        if(node == NULL) {
            return NULL;
    }}}

    check_error
    return node;
}

Node* Parser :: getLet() {
    Node* node = NULL;

    if(cur->type == Token::type_leteral) {
        node = TreeNodeCreate(m_tree, Node::TYPE_LET, cur->data, cur->data_len, NULL, NULL);
        cur ++;
    } else {
        return NULL;
    }
    return node;

}

Node* Parser :: getCall() {
    Node *node = NULL;

    Node *root = NULL;

    if(cur->type == Token::type_id /* have this func */) {
        Token *old = cur;
        cur ++;

        if(cur->type == Token::type_separator && cur->val == '(') {
            cur ++;
        } else {
            return NULL;
        }

        Node* node_1 = NULL;
        Node* node_2 = NULL;

        node = TreeNodeCreate(m_tree, Node::TYPE_CALL, old->data, old->data_len, NULL, NULL);
        root = node;

        int n = 0;
        do {
            node_2 = TreeNodeCreate(m_tree, Node::TYPE_ARG, "arg", 3, NULL, NULL);

            node->right = node_2;
            node_2->par = node;

            if(n > 0) {
                if(cur->type == Token::type_separator && cur->val == ',') {
                    cur ++;
                } else {
                    break;
                }
            }

            node_1 = getE();

            if(!node_1)
                break;

            node_2->left = node_1;
            node_1->par = node_2;

            node = node_2;
            n++;
        } while(true);

        if(cur->type == Token::type_separator && cur->val == ')') {
            cur ++;
        } else {
            return NULL;
        }
    } else {
        return NULL;
    }

    return root;
}

Node* Parser :: getFreeCall() {
    Node *node_1 = getCall();

    if(!node_1)
        return NULL;

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }

    Node *node = TreeNodeCreate(m_tree, Node::TYPE_FREE_CALL, "free call", 9, node_1, NULL);
    node_1->par = node;

    return node;
}

Node* Parser :: getIf() {
    Node *node = NULL;

    if(cur->type == Token::type_keyword && cur->val == 4) {
        node = TreeNodeCreate(m_tree, Node::TYPE_IF, "if", 2, NULL, NULL);
        cur ++;

        if(cur->type == Token::type_separator && cur->val == '(') {
            cur ++;
        } else {
            return NULL;
        }

        Node* node_1 = getE();

        if(cur->type == Token::type_separator && cur->val == ')') {
            cur ++;
        } else {
            return NULL;
        }
        if(cur->type == Token::type_separator && cur->val == '{') {
            cur ++;
        } else {
            return NULL;
        }

        Node *node_2 = getCode();

        if(cur->type == Token::type_separator && cur->val == '}') {
            cur ++;
        } else {
            return NULL;
        }

        node->left = node_1;
        node->right = node_2;

        node_1->par = node;
        node_2->par = node;
    } else {
        return NULL;
    }

    return node;
}

Node* Parser :: getWh() {
    Node *node = NULL;

    if(cur->type == Token::type_keyword && cur->val == 9) {
        node = TreeNodeCreate(m_tree, Node::TYPE_WH, "while", 2, NULL, NULL);
        cur ++;

        if(cur->type == Token::type_separator && cur->val == '(') {
            cur ++;
        } else {
            return NULL;
        }

        Node* node_1 = getE();

        if(cur->type == Token::type_separator && cur->val == ')') {
            cur ++;
        } else {
            return NULL;
        }
        if(cur->type == Token::type_separator && cur->val == '{') {
            cur ++;
        } else {
            return NULL;
        }

        Node *node_2 = getCode();

        if(cur->type == Token::type_separator && cur->val == '}') {
            cur ++;
        } else {
            return NULL;
        }

        node->left = node_1;
        node->right = node_2;

        node_1->par = node;
        node_2->par = node;
    } else {
        return NULL;
    }

    return node;
}


Node* Parser :: getRet() {
    Node* node_1 = NULL;

    if(cur->type == Token::type_keyword && cur->val == 5) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_RET,cur->data, cur->data_len, NULL, NULL);
        cur++;
    } else {
        return NULL;
    }

    Node* node_2 = getE();

    node_1->left = node_2;
    node_2->par = node_1;

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }

    return node_1;
}

Node* Parser :: getPri() {
    Node* node_1 = NULL;

    if(cur->type == Token::type_keyword && cur->val == 6) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_PRI, cur->data, cur->data_len, NULL, NULL);
        cur++;
    } else {
        return NULL;
    }

    Node* node_2 = getE();

    node_1->left = node_2;
    node_2->par = node_1;

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }

    return node_1;
}

Node* Parser :: getSca() {
    Node* node_1 = NULL;

    if(cur->type == Token::type_keyword && cur->val == 7) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_SCA, cur->data, cur->data_len, NULL, NULL);
        cur++;
    } else {
        return NULL;
    }

    Node* node_2 = getVar();

    node_1->left = node_2;
    node_2->par = node_1;

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }

    return node_1;
}

Node* Parser :: getSqrt() {


    Node* node_1 = NULL;

    if(cur->type == Token::type_keyword && cur->val == 8) {
        node_1 = TreeNodeCreate(m_tree, Node::TYPE_SQRT, cur->data, cur->data_len, NULL, NULL);
        cur++;
    } else {
        return NULL;
    }

    Node* node_2 = getE();

    node_1->left = node_2;
    node_2->par = node_1;

    if(cur->type == Token::type_separator && cur->val == ';') {
        cur ++;
    } else {
        return NULL;
    }

    return node_1;
}

