#include "Tree.h"

void NodeCtor(Node* node) {
    assert(node);
}

void NodeCtor(Node* node, t_data* data, Node* left, Node* right, Node* par, int id) {
    assert(node);

    node->data = data;
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

    tree->data = new int[Tree::DATA_SIZE];
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

int get_way_flag(t_data* cur_data, t_data* new_data) {
    if(*new_data >= *cur_data)
        return RIGHT;
    else
        return LEFT;
}

int TreeGoDown(Tree *tree, Node* *cur, t_data* data, int* flag) {
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
        std::cout << "-----------------------[ERROR]-----------------------";
        int d_root = tree->root - tree->nodes;

        tree->nodes_size *= 2;
        tree->nodes = (Node*)realloc(tree->nodes, tree->nodes_size * sizeof(Node));

        tree->root = tree->nodes + d_root;
    }

    NodeCtor(tree->nodes + tree->nodes_num, NULL, NULL, NULL, NULL, tree->nodes_num);

    tree->nodes_num ++;

    if(tree->nodes_num == 1) {
        tree->root = tree->nodes;
    }

    return tree->nodes + tree->nodes_num - 1;
}

int n = 0;

Node* TreeNodeCreate(Tree *tree, int _type, int _data, Node *left, Node *right) {
    assert(tree);

    Node *lst = tree->nodes;

    Node *cur = TreeNodeCreate(tree);

    cur->data = tree->data + tree->data_size;
    tree->data_size ++;

    cur->type = _type;
    *(cur->data) = _data;


    if(left != NULL)
        set_cnt(cur, LEFT, left - lst + tree->nodes);

    if(right != NULL)
        set_cnt(cur, RIGHT, right - lst + tree->nodes);

    return cur;
}

Node* TreePush(Tree* tree, t_data* data) {
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

    NodeCtor(tree->nodes + tree->nodes_num, data, NULL, NULL, NULL, tree->nodes_num);
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
}

int NodeRead(Tree *tree, Node* cur, char *buf, int flag) {
    std::cout << buf << std::endl;

    char *_buf = buf;

    Node* lst_cur = cur;
    Node* lst_nodes = tree->nodes;

    Node *n_node = TreeNodeCreate(tree, 0, 0, NULL, NULL);

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

#define ZERO    Node::TYPE_DIG, 0,  NULL, NULL
#define ONE     Node::TYPE_DIG, 1,  NULL, NULL
#define M_ONE   Node::TYPE_DIG, -1, NULL, NULL
#define TWO     Node::TYPE_DIG, 2,  NULL, NULL

Node *Diff(Tree*, const Node*);
Node *Copy(Tree*, const Node*);

#define L   Copy(tree, node->left)
#define R   Copy(tree, node->right)
#define dL  Diff(tree, node->left)
#define dR  Diff(tree, node->right)
#define OPR    Node::TYPE_OPR

#define Create TreeNodeCreate

Node *Diff(Tree *tree, const Node *node) {
    Node* returned = NULL;

    switch(node->type){
    case Node::TYPE_DIG    :   returned = Create(tree, ZERO); break;
    case Node::TYPE_VAR    :   returned = Create(tree, ONE); break;
    case Node::TYPE_OPR    :   switch(*node->data) {
                                case '+' : returned = Create(tree, OPR, '+', dL, dR); break;
                                case '-' : returned = Create(tree, OPR, '-', dL, dR); break;
                                case '*' : returned = Create(tree, OPR, '+', Create(tree, OPR, '*', dL, R),
                                                                             Create(tree, OPR, '*', L, dR)); break;
                                case '/' : returned = Create(tree, OPR, '/', Create(tree, OPR, '-', Create(tree, OPR, '*', dL, R), Create(tree, OPR, '*', L, dR)),
                                                                             Create(tree, OPR, '*', R, R)); break;
                                case 's' : returned = Create(tree, OPR, '*', Create(tree, OPR, 'c', L, NULL), dL); break;
                                case 'c' : returned = Create(tree, OPR, '*', Create(tree, OPR, '*', Create(tree, M_ONE), Create(tree, OPR, 's', L, NULL)),
                                                                             dL); break;
                                case 'L' : returned = Create(tree, OPR, '*', Create(tree, OPR, '/', Create(tree, ONE), L),
                                                                             dL); break;
                                case 'e' : returned = Create(tree, OPR, '*', Create(tree, OPR, 'e', L, NULL),
                                                                             dL); break;
                                case '^' : returned = Create(tree, OPR, '*', Create(tree, OPR, '^', L, R),
                                                                             Create(tree, OPR, '+', Create(tree, OPR, '/', Create(tree, OPR, '*', dL,
                                                                                                                                                  R),
                                                                                                                           L),
                                                                                                    Create(tree, OPR, '*', dR,
                                                                                                                           Create(tree, OPR, 'L', L,
                                                                                                                                                  NULL)))); break;
                                case 'a' : returned =  Create(tree, OPR, '*', Create(tree, OPR, '/', Create(tree, ONE),
                                                                                                     Create(tree, OPR, '^', Create(tree, OPR, '-', Create(tree, ONE),
                                                                                                                                                   Create(tree, OPR, '^', L, Create(tree, TWO))),
                                                                                                                            Create(tree, OPR, '/', Create(tree, ONE),
                                                                                                                                                   Create(tree, TWO)))),
                                                                              dL); break;
                                case 'S' : returned = Create(tree, OPR, '*', Create(tree, OPR, 'C', L, NULL), dL); break;
                                case 'C' : returned = Create(tree, OPR, '*', Create(tree, OPR, 'S', L, NULL), dL); break;
                                }
    }

    tree->root = returned;
    return returned;
}

#undef Create
#undef L
#undef dL
#undef R
#undef dR
#undef ZERO
#undef ONE

Node *Copy(Tree *tree, const Node *n) {
    Node *cur = TreeNodeCreate(tree, 0, 0, NULL, NULL);
    *(cur->data) = *(n->data);
    cur->type = n->type;

    if(n->left != NULL)
        cur->left = Copy(tree, n->left);

    if(n->right != NULL)
        cur->right = Copy(tree, n->right);

    cur->par = NULL;

    return cur;
}

int NodeDelete0MultSmth(Node* n) {
    assert(n);

    int count = 0;

    if(n->left)
        count += NodeDelete0MultSmth(n->left);
    if(n->right)
        count += NodeDelete0MultSmth(n->right);

    if(n->type == Node::TYPE_OPR && *(n->data) == '*') {
        if((n->left ->type == Node::TYPE_DIG && *(n->left ->data) == 0) ||
           (n->right->type == Node::TYPE_DIG && *(n->right->data) == 0)) {

            NodeDelete(n ->left); n ->left = NULL;
            NodeDelete(n->right); n->right = NULL;

            n->type = Node::TYPE_DIG;
            *(n->data) = 0;
            count ++;
        }
    }

    return count;
}

int NodeDelete1MultSmth(Node* n) {
    assert(n);

    int count = 0;

    if(n->left)
        count += NodeDelete1MultSmth(n->left);
    if(n->right)
        count += NodeDelete1MultSmth(n->right);

    if(n->type == Node::TYPE_OPR && *(n->data) == '*') {
        if(n-> left ->type == Node::TYPE_DIG && *(n-> left ->data) == 1) {
            NodeDelete(n-> left);

            n->type = n->right->type;
            *(n->data) = *(n->right->data);
            n-> left = n->right-> left;

            Node* n_right = n->right->right;

            NodeDelete(n->right);

            n->right = n_right;
            count ++;
        } else if(n->right ->type == Node::TYPE_DIG && *(n->right->data) == 1) {
            NodeDelete(n->right);

            n->type = n-> left->type;
            *(n->data) = *(n-> left->data);
            n->right = n-> left->right;

            Node* n_left = n-> left-> left;

            NodeDelete(n-> left);

            n->left = n_left;
            count ++;
        }
    }

    return count;
}

int NodeDelete0PlusSmth(Node* n) {
    assert(n);

    int count = 0;

    if(n->left)
        count += NodeDelete0PlusSmth(n->left);
    if(n->right)
        count += NodeDelete0PlusSmth(n->right);

    if(n->type == Node::TYPE_OPR && *(n->data) == '+') {
        if(n-> left ->type == Node::TYPE_DIG && *(n-> left ->data) == 0) {
            NodeDelete(n-> left);

            n->type = n->right->type;
            *(n->data) = *(n->right->data);

            n-> left = n->right-> left;
            Node* n_right = n->right->right;

            NodeDelete(n->right);

            n->right = n_right;
            count ++;
        } else if(n->right ->type == Node::TYPE_DIG && *(n->right->data) == 0) {
            NodeDelete(n->right);

            n->type = n-> left->type;
            *(n->data) = *(n-> left->data);
            n->right = n-> left->right;

            Node* n_left = n-> left-> left;

            NodeDelete(n-> left);

            n->left = n_left;
            count ++;
        }
    }

    return count;
}

int NodeDelete0DivSmth(Node* n) {
    assert(n);

    int count = 0;

    if(n->left)
        count += NodeDelete0DivSmth(n->left);
    if(n->right)
        count += NodeDelete0DivSmth(n->right);

    if(n->type == Node::TYPE_OPR && *(n->data) == '/') {
        if(n->left ->type == Node::TYPE_DIG && *(n->left ->data) == 0) {
            NodeDelete(n ->left); n ->left = NULL;
            NodeDelete(n->right); n->right = NULL;

            n->type = Node::TYPE_DIG;
            *(n->data) = 0;
            count ++;
        }
    }

    return count;
}

int NodeDeleteSmthMin0(Node* n) {
    assert(n);

    int count = 0;

    if(n->left)
        count += NodeDeleteSmthMin0(n->left);
    if(n->right)
        count += NodeDeleteSmthMin0(n->right);

    if(n->type == Node::TYPE_OPR && *(n->data) == '-') {
        if(n->right->type == Node::TYPE_DIG && *(n->right->data) == 0) {
            NodeDelete(n->right);

            n->type = n-> left->type;
            *(n->data) = *(n-> left->data);
            n->right = n-> left->right;

            Node* n_left = n-> left-> left;

            NodeDelete(n-> left);

            n->left = n_left;
            count ++;
        }
    }

    return count;
}

void NodeDumb(Node* node) {
    assert(node);

    printf("\tNode (id=%d), %p {\n", node->id, node);
    printf("\t\ttype: %d\n", node->type);

    if(node->data)
        printf("\t\tdata: %d\n", *(node->data));

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

    if(node->type == Node::TYPE_DIG)
        printf("%d", *node->data);
    else
        printf("%c", (int)*node->data);

    if(node->right) {
        printf("(");
        NodeInfDumb(node->right);
        printf(")");
    }
}

void NodeDelete(Node* node) {
    assert(node);

    printf("DELETE:"); NodeDumb(node);

    NodeDtor(node);
}


void NodeTexDumb(Node* node, FILE *file) {
    assert(node);

    if(node->type == Node::TYPE_OPR) {
        switch(*(node->data)) {
            case '/': fprintf(file, " \\frac{");            NodeTexDumb(node->left, file); fprintf(file, "}{");            NodeTexDumb(node->right, file);   fprintf(file, "} "); return; break;
            case 'c': fprintf(file, " \\cos \\left( ");     NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case 'C': fprintf(file, " \\cosh \\left( ");    NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case 's': fprintf(file, " \\sin \\left( ");     NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case 'S': fprintf(file, " \\sinh \\left( ");    NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case 'L': fprintf(file, " \\ln \\left( ");      NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case 'e': fprintf(file, " e^{ \\left( ");       NodeTexDumb(node->left, file); fprintf(file, "\\right) } ");                                                          return; break;
            case '^': fprintf(file, " \\left( ");           NodeTexDumb(node->left, file); fprintf(file, " \\right) ^ {"); NodeTexDumb(node->right, file);   fprintf(file, "} "); return; break;
            case 'a': fprintf(file, " \\arcsin \\left( ");  NodeTexDumb(node->left, file); fprintf(file, " \\right) ");                                                           return; break;
            case '*':
                bool is_left = false, is_right = false;
                if(node->left->type == Node::TYPE_OPR && (*(node->left->data) == '+' || *(node->left->data) == '-'))
                    is_left = true;

                if(node->right->type == Node::TYPE_OPR && (*(node->right->data) == '+' || *(node->right->data) == '-'))
                    is_right = true;

                if(is_left) fprintf(file, " \\left( ");
                NodeTexDumb(node->left, file);
                if(is_left) fprintf(file, " \\right) ");

                fprintf(file, " \\cdot ");

                if(is_right) fprintf(file, " \\left( ");
                NodeTexDumb(node->right, file);
                if(is_right) fprintf(file, " \\right) ");

                return; break;
        }
    }

    if(node->left){
        NodeTexDumb(node->left, file);
    }

    if(node->type == Node::TYPE_DIG)
        fprintf(file, "%d", *node->data);
    else
        fprintf(file, "%c", (int)*node->data);

    if(node->right){
        NodeTexDumb(node->right, file);
    }

}

void TreeDumb(Tree* tree) {
    printf("Tree {\n");
    printf("\tnodes_size: %d\n", tree->nodes_size);
    printf("\tnodes_num: %d\n", tree->nodes_num);
    printf("\troot: %p\n", tree->root);
    printf("\tnodes: %p\n\n", tree->nodes);

    for(int i = 0; i < tree->nodes_num; i ++) {
        if(tree->nodes + i)
            NodeDumb(tree->nodes + i);
    }
    printf("}\n");


    FILE *out = fopen("out.txt", "w");
    fprintf(out, "digraph G {\n");

    for(int i = 0; i < tree->nodes_num; i ++) {
        if(!(tree->nodes + i))
            continue;
        if(tree->nodes[i].type == Node::TYPE_CLR)
            continue;

        if(tree->nodes[i].type == Node::TYPE_DIG)
            fprintf(out, "node%d[label = \"%d\"];\n", tree->nodes[i].id, *(tree->nodes[i].data));
        else
            fprintf(out, "node%d[label = \"%c\"];\n", tree->nodes[i].id, *(tree->nodes[i].data));
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

    }

    fprintf(out, "}\n");
    fclose(out);
}

void TreeTexDumb(Tree* tree, FILE* file) {
    assert(tree);
    assert(tree->root);

    NodeTexDumb(tree->root, file);
}

void TreeInfDumb(Tree* tree) {
    assert(tree);
    assert(tree->root);

    printf("tree: '");
    NodeInfDumb(tree->root);
    printf("'\n");
}
