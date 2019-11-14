#include "Tree.h"

void NodeCtor(Node* node) {
    assert(node);
}

void NodeCtor(Node* node, char* data, int data_len, Node* left, Node* right, Node* par, int id) {
    assert(node);

    node->data = data;
    node->data_len = data_len;
    node->left = left;
    node->right = right;
    node->par = par;

    node->id = id;
}

void NodeDtor(Node* node) {
    assert(node);

    node->type = Node::TYPE_CLR;
    node->data = NULL;
    node->left = NULL;
    node->right = NULL;
    node->par = NULL;

    node->id = -1;
}

void TreeCtor(Tree* tree) {
    assert(tree);

    tree->data = new char[Tree::DATA_SIZE];
    tree->data_size = 0;

    tree->nodes_size = Tree::MIN_NODES_SIZE;
    tree->nodes = new Node[tree->nodes_size];

    tree->nodes_num = 0;

    tree->root = NULL;
}

void TreeDtor(Tree* tree) {
    assert(tree);

    delete[] tree->nodes;
    delete[] tree->data;
}

Node* get_cnt(const Node *node, int flag) {
    assert(node);
    assert(flag == LEFT ||
           flag == RIGHT ||
           flag == PARENT);

    switch(flag) {
        case LEFT   : return node->left;  break;
        case RIGHT  : return node->right; break;
        case PARENT : return node->par;   break;
    }
}

void set_cnt(Node *node, int flag, Node* _new) {
    assert(node);
    assert(flag == LEFT ||
           flag == RIGHT ||
           flag == PARENT);

    assert(!get_cnt(node, flag));


    switch(flag) {
        case LEFT   : node->left  = _new; break;
        case RIGHT  : node->right = _new; break;
        case PARENT : node->par   = _new; break;
    }

    return;
}

int get_way_flag(char* cur_data, char* new_data) {
    if(*new_data >= *cur_data)
        return RIGHT;
    else
        return LEFT;
}

int TreeGoDown(Tree *tree, Node* *cur, char* data, int* flag) {
    assert(tree);
    assert(cur);
    assert(data);

    if(*cur == NULL) {
        return 0;
    } else {
        int _flag = get_way_flag((*cur)->data, data);
        Node *_cur = *cur;

        *cur = get_cnt(*cur, _flag);

        if(*cur == NULL) {
            *cur = _cur;
            *flag = _flag;
            return 1;
        }

        return TreeGoDown(tree, cur, data, flag);
    }
}

Node* TreeNodeCreate(Tree *tree) {
    assert(tree);
    assert(tree->data);
    assert(tree->nodes);
    assert(tree->nodes_size);

    if(tree->nodes_num >= tree->nodes_size) {
        int d_root = tree->root - tree->nodes;

        tree->nodes_size *= 2;
        tree->nodes = (Node*)realloc(tree->nodes, tree->nodes_size * sizeof(Node));

        tree->root = tree->nodes + d_root;
    }

    NodeCtor(tree->nodes + tree->nodes_num, NULL, 0, NULL, NULL, NULL, tree->nodes_num);

    tree->nodes_num ++;

    if(tree->nodes_num == 1) {
        tree->root = tree->nodes;
    }

    return tree->nodes + tree->nodes_num - 1;
}

int n = 0;

Node* TreeNodeCreate(Tree *tree, int _type, char* data, int data_size, Node *left, Node *right) {
    assert(tree);

    Node *lst = tree->nodes;

    Node *cur = TreeNodeCreate(tree);

    cur->data = tree->data + tree->data_size;
    cur->data_len = data_size;
    tree->data_size += data_size + 1;

    cur->type = _type;

    for(int i = 0; i < data_size; i ++) {
        cur->data[i] = data[i];
    }
    cur->data[data_size] = '\0';

    if(left != NULL)
        set_cnt(cur, LEFT, left - lst + tree->nodes);

    if(right != NULL)
        set_cnt(cur, RIGHT, right - lst + tree->nodes);

    return cur;
}

Node* TreePush(Tree* tree, char* data, int data_len) {
    assert(tree);
    assert(data);
    assert(tree->nodes);
    assert(tree->nodes_size);

    if(tree->nodes_num >= tree->nodes_size) {
        int d_root = tree->root - tree->nodes;

        tree->nodes_size *= 2;
        tree->nodes = (Node*)realloc(tree->nodes, tree->nodes_size * sizeof(Node));

        tree->root = tree->nodes + d_root;
    }

    NodeCtor(tree->nodes + tree->nodes_num, data, data_len, NULL, NULL, NULL, tree->nodes_num);
    tree->nodes_num ++;

    Node* new_node = tree->nodes + tree->nodes_num - 1;

    if(tree->nodes_num == 1) {
        tree->root = tree->nodes;
    } else {
        Node *cur = tree->root;
        int flag = 0;

        assert(TreeGoDown(tree, &cur, data, &flag));

        set_cnt(cur, flag, new_node);
        set_cnt(new_node, PARENT, cur);
    }

    return new_node;
}

int scan_dig(char* buf, int *data) {
    int dig = 0;
    int n_start = 0, n_end = 0;

    if(sscanf(buf, "%n%d%n", &n_start, &dig, &n_end) == 0) {
        return 0;
    }

    *data = dig;

    return n_end - n_start;
}

int scan_opr(char* buf, int *data) {
    int opr = 0;
    sscanf(buf, "%c", &opr);

    if((opr != '+') &&
       (opr != '-') &&
       (opr != '*') &&
       (opr != '/') &&
       (opr != 'c') &&  // cos
       (opr != 'C') &&  // ch
       (opr != 's') &&  // sin
       (opr != 'S') &&  // sh
       (opr != 'L') &&  // ln
       (opr != 'e') &&  // exp
       (opr != '^') &&  // pow
       (opr != 'a')) {  // arcsin
        return 0;
    }

    *data = opr;

    return 1;
}

int scan_var(char* buf, int *data) {
    int var = 0;
    sscanf(buf, "%c", &var);

    if((var != 'x')) {
        return 0;
    }

    *data = var;

    return 1;
}

int scan_data(char** buf, Node *node) {
    int data = 0;
    /*
    int dig_l = scan_dig(*buf, &data);
    int opr_l = scan_opr(*buf, &data);
    int var_l = scan_var(*buf, &data);

    int len = 0;

    if(dig_l != 0 &&
       opr_l == 0 &&
       var_l == 0) {
        len = dig_l;
        node->type = Node::TYPE_DIG;
    }

    if(dig_l == 0 &&
       opr_l != 0 &&
       var_l == 0) {
        len = opr_l;
        node->type = Node::TYPE_OPR;
    }

    if(dig_l == 0 &&
       opr_l == 0 &&
       var_l != 0) {
        len = var_l;
        node->type = Node::TYPE_VAR;
    }

    *(node->data) = data;

    return len;
*/
}

int NodeRead(Tree *tree, Node* cur, char *buf, int flag) {
    std::cout << buf << std::endl;

    char *_buf = buf;

    Node* lst_cur = cur;
    Node* lst_nodes = tree->nodes;

    Node *n_node = TreeNodeCreate(tree, 0, "", 0, NULL, NULL);

    cur = ((lst_cur - lst_nodes) + tree->nodes);

    if(cur != NULL) {
        set_cnt(cur, flag, n_node);
    }

    if(*buf == '(') {
        buf ++;

        buf += NodeRead(tree, n_node, buf, LEFT);

        assert(*buf == ')');
        buf ++;
    }

    int len = scan_data(&buf, n_node);
    assert(len);

    buf += len;

    if(cur != NULL)
        n_node->par = cur;

    if(*buf == '(') {
        buf ++;
        buf += NodeRead(tree, n_node, buf, RIGHT);

        assert(*buf == ')');
        buf ++;
    }
    return buf - _buf;
}

void TreeCreate(Tree *tree, char *buf) {
    assert(tree);
    assert(buf);

    NodeRead(tree, NULL, buf, 0);
}

Node *Copy(Tree *tree, const Node *n) {
    Node *cur = TreeNodeCreate(tree, 0, "", 0, NULL, NULL);
    *(cur->data) = *(n->data);
    cur->type = n->type;

    if(n->left != NULL)
        cur->left = Copy(tree, n->left);

    if(n->right != NULL)
        cur->right = Copy(tree, n->right);

    cur->par = NULL;

    return cur;
}

int NodeNumbering(Node* node, int type, int *num) {
    if(node->type == type) {
        node->val = *num;
        (*num) ++;
    }

    if(node->left) {
        NodeNumbering(node->left, type, num);
    }
    if(node->right) {
        NodeNumbering(node->right, type, num);
    }
}

#define GO_UP return searchDeclarationEarlier(node->par, type, data, data_len);

Node* searchDeclarationEarlier(Node* node, int type, char* data, int data_len) {
    assert(node);
    assert(data);

    if(type == Node::TYPE_VAR) { ///  ------------ VARS ----------------

        if(!node->par && node->type == Node::TYPE_DEC_FU) {
            return NULL;
        }

        if(node->type != Node::TYPE_CODE &&
           node->type != Node::TYPE_DEC_VAR &&
           node->type != Node::TYPE_FU) {
            GO_UP
        }

        if(node->type == Node::TYPE_FU) {
            Node* arg = node->left;

            while(arg->left) {
                Node *var = arg->left->left;

                if(var->data == data) {
                    arg = arg->right;
                    continue;
                }

                bool declar = _strCmp(var->data, var->data_len,
                                           data,      data_len);

                if(declar)
                    return var;

                arg = arg->right;
            }

            GO_UP
        }

        if(node->type == Node::TYPE_DEC_VAR){
            Node* var = node->left;
            if(var->data == data) {
                if(node->par)
                    GO_UP
                else
                    return NULL;
            }

            bool declar = _strCmp(var->data, var->data_len,
                                       data,      data_len);

            if(declar) {
                return var;
            } else {
                if(node->par)
                    GO_UP
                else
                    return NULL;
            }
        }


        if(node->type == Node::TYPE_CODE){
            if(node->left->type == Node::TYPE_DEC_VAR) {
                Node* var = node->left->left;

                if(var->data == data)
                    GO_UP

                bool declar = _strCmp(var->data, var->data_len,
                                           data,      data_len);
                if(declar)
                    return node->left->left;
                else
                    GO_UP
            } else {
                GO_UP
            }
        }

        return NULL;
    } else if(type == Node::TYPE_FU) { ///  ------------ FUNCTIONS ------------------

        if(!node->par && node->type == Node::TYPE_DEC_VAR) {
            return NULL;
        }

        if(node->type != Node::TYPE_DEC_FU) {
            GO_UP
        }

        if(node->type == Node::TYPE_DEC_FU){
            Node* var = node->left;
            if(var->data == data) {
                if(node->par)
                    GO_UP
                else
                    return NULL;
            }

            bool declar = _strCmp(var->data, var->data_len,
                                       data,      data_len);

            if(declar) {
                return var;
            } else {
                if(node->par)
                    GO_UP
                else
                    return NULL;
            }
        }

        return NULL;
    }

    return NULL;
}

#undef GO_UP

void NodeDumb(Node* node) {
    assert(node);

    printf("\tNode (id=%d), %p {\n", node->id, node);
    printf("\t\ttype: %d\n", node->type);

    if(node->data)
        printf("\t\tdata: '%s'\n", node->data);

    if(node->left)
        printf("\t\tleft: %p\n", node->left);

    if(node->right)
        printf("\t\trigh: %p\n", node->right);

    if(node->par)
        printf("\t\tpare: %p\n", node->par);

    printf("\t}\n");
}

void NodeInfDumb(Node* node) {
    assert(node);

    if(node->left) {
        printf("(");
        NodeInfDumb(node->left);
        printf(")");
    }

    if(node->data)
        printf("'%s'", node->data);

    if(node->right) {
        printf("(");
        NodeInfDumb(node->right);
        printf(")");
    }
}

void NodePostDumb(Node* node) {
    assert(node);

    if(node->left) {
        printf("(");
        NodePostDumb(node->left);
        printf(")");
    }
    if(node->right) {
        printf("(");
        NodePostDumb(node->right);
        printf(")");
    }

    if(node->data)
        printf("%s", node->data);
}

#define DUMB_L \
    if(node->left) {\
        NodeAssDumb(node->left, file);\
    }

#define DUMB_R \
    if(node->right) {\
        NodeAssDumb(node->right, file);\
    }

#define DUMB DUMB_L DUMB_R

void NodeAssDumb(Node* node, FILE *file) {
    assert(node);

    switch(node->type) {
    case Node::TYPE_DEC_VAR:
        DUMB_R
        break;
    case Node::TYPE_VAR:
        if(node->val >= 0) { /// local
            fprintf(file, "rpop RAX\n");
            fprintf(file, "push %d\n", node->val);
            fprintf(file, "add\n");
            fprintf(file, "rapo\n\n", node->data);
        }
        if(node->val < 0) { /// global
            fprintf(file, "rpop RA%c\n", 'A' -(node->val + 1));
        }
        break;
    case Node::TYPE_DEC_FU:
        DUMB
        break;
    case Node::TYPE_FU:
        fprintf(file, "lbl fu%d\n", node->val);
        DUMB
        break;
    case Node::TYPE_ARG:
        DUMB
        break;
    case Node::TYPE_DEC_ARG:
        if(node->left->val >= 0) { /// local
            fprintf(file, "rpop RAX\n");
            fprintf(file, "push %d\n", node->left->val);
            fprintf(file, "add\n");
            fprintf(file, "rapu\n", node->left->data);
        }
        if(node->left->val < 0) { /// global
            fprintf(file, "rpush RA%c\n", 'A' -(node->left->val + 1));
        }

        break;
    case Node::TYPE_CODE:
        DUMB
        break;
    case Node::TYPE_ASS:
        DUMB_R

        if(node->left->val >= 0) { /// local
            fprintf(file, "rpop RAX\n");
            fprintf(file, "push %d\n", node->left->val);
            fprintf(file, "add\n");
            fprintf(file, "rapu\n", node->left->data);
        }
        if(node->left->val < 0) { /// global
            fprintf(file, "rpush RA%c\n", 'A' -(node->left->val + 1));
        }

        break;
    case Node::TYPE_OPR:
        DUMB
        #define exch(symb, str) \
            case symb: fprintf(file, "%s\n", str); break;
        switch(node->data[0]) {
            exch('+', "add");
            exch('-', "sub");
            exch('*', "mul");
            exch('/', "div");
            exch('>', "more");
            exch('<', "less");
            exch('~', "eql");
        }
        #undef exch
        break;
    case Node::TYPE_LET:
        fprintf(file, "push %s\n", node->data);
        break;
    case Node::TYPE_CALL:
        fprintf(file, "\n");
        DUMB_R
        fprintf(file, "call fu%d\n\n", node->val);
        break;
    case Node::TYPE_FREE_CALL:
        DUMB_L
        fprintf(file, "pop\n");
        break;
    case Node::TYPE_IF:
        fprintf(file, "\n");
        DUMB_L
        fprintf(file, "push 1\n");
        fprintf(file, "je ifs%d\n", node->val);
        fprintf(file, "jmp ife%d\n", node->val);
        fprintf(file, "lbl ifs%d\n", node->val);
        DUMB_R
        fprintf(file, "\nlbl ife%d\n", node->val);
        break;
    case Node::TYPE_WH:
        fprintf(file, "\n");
        fprintf(file, "lbl whs1%d\n", node->val);
        DUMB_L
        fprintf(file, "push 1\n");
        fprintf(file, "je whs2%d\n", node->val);
        fprintf(file, "jmp whe%d\n", node->val);
        fprintf(file, "lbl whs2%d\n", node->val);
        DUMB_R

        fprintf(file, "\jmp whs1%d\n", node->val);
        fprintf(file, "\nlbl whe%d\n", node->val);
        break;
    case Node::TYPE_RET:
        DUMB_L
        fprintf(file, "ret\n\n");
        break;
    case Node::TYPE_PRI:
        DUMB_L
        fprintf(file, "out\n");
        break;
    case Node::TYPE_SCA:
        fprintf(file, "in\n");

        if(node->left->val >= 0) { /// local
            fprintf(file, "rpop RAX\n");
            fprintf(file, "push %d\n", node->left->val);
            fprintf(file, "add\n");
            fprintf(file, "rapu\n", node->left->data);
        }
        if(node->left->val < 0) { /// global
            fprintf(file, "rpush RA%c\n", 'A' -(node->left->val + 1));
        }
        break;
    case Node::TYPE_SQRT:
        fprintf(file, "in\n");

        if(node->left->val >= 0) { /// local
            fprintf(file, "rpop RAX\n");
            fprintf(file, "push %d\n", node->left->val);
            fprintf(file, "add\n");
            fprintf(file, "rapu\n", node->left->data);
        }
        if(node->left->val < 0) { /// global
            fprintf(file, "rpush RA%c\n", 'A' -(node->left->val + 1));
        }
        break;

    }
}

#undef DUMB
#undef DUMB_L
#undef DUMB_R

void NodeDelete(Node* node) {
    assert(node);

    NodeDtor(node);
}

void TreeDumb(Tree* tree) {

    /*printf("Tree {\n");
    printf("\tnodes_size: %d\n", tree->nodes_size);
    printf("\tnodes_num: %d\n", tree->nodes_num);
    printf("\troot: %p\n", tree->root);
    printf("\tnodes: %p\n\n", tree->nodes);

    for(int i = 0; i < tree->nodes_num; i ++) {
        if(tree->nodes + i)
            NodeDumb(tree->nodes + i);
    }
    printf("}\n");
*/

    FILE *out = fopen("out.txt", "w");
    fprintf(out, "digraph G {\n");

    for(int i = 0; i < tree->nodes_num; i ++) {
        if(!(tree->nodes + i))
            continue;
        if(tree->nodes[i].type == Node::TYPE_CLR)
            continue;

        fprintf(out, "node%d[label = \"%s,%d, %d\"", tree->nodes[i].id, tree->nodes[i].data, tree->nodes[i].val, tree->nodes[i].id);
        //fillcolor = blue, style=filled
        switch(tree->nodes[i].type) {
            case Node::TYPE_DEC_VAR:    fprintf(out, "fillcolor = greenyellow   , style=filled"); break;
            case Node::TYPE_VAR:        fprintf(out, "fillcolor = greenyellow   , style=filled"); break;
            case Node::TYPE_DEC_ARG:    fprintf(out, "fillcolor = green         , style=filled"); break;
            case Node::TYPE_FU:         fprintf(out, "fillcolor = cyan          , style=filled"); break;
            case Node::TYPE_ASS:        fprintf(out, "fillcolor = gold          , style=filled"); break;
            case Node::TYPE_OPR:        fprintf(out, "fillcolor = peru          , style=filled"); break;
            case Node::TYPE_LET:        fprintf(out, "fillcolor = orange        , style=filled"); break;
            case Node::TYPE_CALL:       fprintf(out, "fillcolor = lightskyblue  , style=filled"); break;
            case Node::TYPE_RET:        fprintf(out, "fillcolor = red           , style=filled"); break;
        }

        fprintf(out, "];\n");
    }
    for(int i = 0; i < tree->nodes_num; i ++) {
        if(!(tree->nodes + i))
            continue;
        if(tree->nodes[i].type == Node::TYPE_CLR)
            continue;

        if(tree->nodes[i].right != NULL) {
            fprintf(out, "\"node%d\":f0 -> \"node%d\":f0;\n", tree->nodes[i].id, tree->nodes[i].right->id);
        }
        if(tree->nodes[i].left != NULL) {
            fprintf(out, "\"node%d\":f0 -> \"node%d\":f0;\n", tree->nodes[i].id, tree->nodes[i].left->id);
        }

        if(tree->nodes[i].par != NULL) {
            fprintf(out, "\"node%d\":f0 -> \"node%d\":f0;\n", tree->nodes[i].id, tree->nodes[i].par->id);
        }


    }

    fprintf(out, "}\n");
    fclose(out);
}

void TreeInfDumb(Tree* tree) {
    assert(tree);
    assert(tree->root);

    printf("tree: '");
    NodeInfDumb(tree->root);
    printf("'\n");
}

void TreePostDumb(Tree* tree) {
    assert(tree);
    assert(tree->root);

    printf("tree: '");
    NodePostDumb(tree->root);
    printf("'\n");
}

void TreeAssDumb(Tree* tree, FILE *file) {
    assert(tree);
    assert(tree->root);

    NodeAssDumb(tree->root, file);
}
