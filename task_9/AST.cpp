#include "Token.h"
#include "Tree.h"
#include "RDP.h"
#include "AST.h"

AST :: AST() {
    token_n = 0;
    max_token_n = 0;
    source_len = 0;

    TreeCtor(&m_tree);
    m_parser = Parser();

    m_parser.m_tree = &m_tree;
}

AST :: ~AST() {}

void AST :: initSource(const char* path) {
    printf(" [START]\t" "init source" "\n");

    if(!source) {
        deleteSource();
    }

    assert(path);
    printf("[MESSG]\t" "file path: '%s'" "\n", path);

    FILE* file = fopen(path, "r");
    assert(file);

    readFile(file, &source, &source_len);

    fclose(file);
    printf(" [FINSH]\t" "init source" "\n");
}

void AST :: deleteSource() {
    printf(" [START]\t" "delete source" "\n");

    delete[] source;

    printf(" [FINSH]\t" "delete source" "\n");
}

void AST :: deleteTokens() {
    delete[] token;
    max_token_n = 0;
    token_n = 0;
}

Token* AST :: addToken(Token _token) {
    if(_token.type == Token::type_comment)
        return NULL;

    if(max_token_n == 0) {
        max_token_n = MIN_MAX_TOKEN_N;
        token = (Token*)calloc(max_token_n, sizeof(Token));
    }

    if(token_n >= max_token_n) {
        max_token_n *= 2;
        token = (Token*)realloc(token, max_token_n * sizeof(Token));
    }

    token[token_n] = _token;

    token_n ++;

    return token + token_n - 1;
}

int AST :: useLexer() {
    printf(" [START]\t" "Lexer working" "\n");

    if(token_n) {
        deleteTokens();
    }

    char* cur = source;

    while(*cur != '\0') {
        if(cur - source >= source_len){
            printf("[ERROR]\t" "cur-source=%d > source_len=%d" "\n", cur-source, source_len);
            return 0;
        }

        int skip_val = Token::skipIgnorSymbols(cur);
        cur += skip_val;

        if(*cur == '\0')
            break;

        Token n_token = Token();
        int val = Token::getToken(cur, &n_token);

        if(!val) {
            printf("[ERROR]\t" "did't find any token '%15s'" "\n", cur);
            return 0;
        }


        addToken(n_token);
        cur += val;
    }

    Token end_token = Token();
    end_token.type = Token::type_end;
    end_token.data = cur;

    addToken(end_token);

    printf(" [FINSH]\t" "Lexer working" "\n");

    return 1;
}

int AST :: useParser() {
    printf(" [START]\t" "Parser working" "\n");

    m_parser.getG(token);

    printf(" [FINSH]\t" "Parser working" "\n");
    return 1;
}

int AST :: useChecker() {
    printf(" [START]\t" "Checker working" "\n");

    numberingIf();

    numberingVars();
    checkFuncs();
    connectVars();

    checkVars();

    printf("[MESSG] %d function:\n", n_func);
    for(int i = 0; i < n_func; i ++) {
        printf("\t'%s()' with %d args\n", funcs[i].m_node->data, funcs[i].arg_n);
    }

    printf("[MESSG] %d vars:\n", n_var);
    for(int i = 0; i < n_var; i ++) {
        printf("\t'%s' ", vars[i].m_node->data);
        if(vars[i].m_fu) {
            printf("is in func '%s()'", vars[i].m_fu->m_node->data);
        } else{
            printf("is global (I see everythings!!!)");
        }

        printf("\n");
    }

    if(n_global_vars > MAX_GLOBAL_VARS_NUM) {
        printf("[ERROR] there are too many global vars!!!?\n");
        return 0;
    }


    printf(" [FINSH]\t" "Checker working" "\n");
}

int AST :: numberingIf() {
    int n = 1;
    NodeNumbering(m_tree.root, Node::TYPE_IF, &n);
}

int AST :: numberingVars() {
    delete[] vars;
    n_var = 0;

    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type == Node::TYPE_DEC_VAR ||
           node->type == Node::TYPE_DEC_ARG) {
            n_var ++;
        }
    }

    vars = new VarInfo[n_var];

    int counter = 0;
    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type == Node::TYPE_DEC_VAR ||
           node->type == Node::TYPE_DEC_ARG) {

            vars[counter].m_node = node->left;
            vars[counter].m_fu = NULL;

            node->left->val = counter;

            counter ++;
        }
    }
}

int AST :: checkFuncs() {
    delete[] funcs;
    n_func = 0;

    main_n = 0;

    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type == Node::TYPE_DEC_FU) {
            node->left->val = n_func;

            if(_strCmp(node->left->data, node->left->data_len, "main", 4)){
                main_n = n_func;
            }

            n_func ++;
        }
    }

    if(main_n == -1) {
        printf("[ERROR] main() isn't declared\n");
        return 0;
    }

    funcs = new FuInfo[n_func];

    int counter = 0;
    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type == Node::TYPE_DEC_FU) {
            funcs[counter].m_node = node->left;
            funcs[counter].arg_n = 0;

            funcs[counter].var_n_max = 0;
            funcs[counter].var_n_min = n_var;

            counter ++;
        }
    }

    for(int i = 0; i < n_func; i ++) {
        Node *arg = funcs[i].m_node->left;

        while(arg->left) {
            funcs[i].arg_n ++;
            arg = arg->right;
        }
    }

    if(funcs[main_n].arg_n != 0) {
        printf("[ERROR] main() shouldn't have any arguments\n");
        return 0;
    }


    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type != Node::TYPE_FU &&
           node->type != Node::TYPE_CALL) {
            continue;
        }

        Node* dec_node = searchDeclarationEarlier(node->par, Node::TYPE_FU, node->data, node->data_len);

        if(node->par->type == Node::TYPE_DEC_FU) {
            if(dec_node) {
                printf("[ERROR] redeclaration function '%s()'\n", node->data);
                NodeDumb(node);
                return 0;
            } else {
                continue;
            }
        }
        /// only CALL

        if(!dec_node) {
            printf("[ERROR] function '%s()' isn't declared\n", node->data);
            NodeDumb(node);
            return 0;
        }

        node->val = dec_node->val;

        int n_args = 0;
        Node* arg = node->right;
        while(arg->left){
            n_args ++;
            arg = arg->right;
        }

        if(n_args != funcs[node->val].arg_n) {
            printf("[ERROR] wrong number arguments (%d) in '%s()' function call\n", n_args, node->data);
            NodeDumb(node);
            return 0;
        }
    }

    return 1;
}

int AST :: connectVars() {
    for(int i = 0; i < n_var; i ++) {
        Node* node = vars[i].m_node->par;

        while(node->par && node->par->type != Node::TYPE_FU) {
            node = node->par;
        }

        if(node->par)
           vars[i].m_fu = funcs + node->par->val;
    }

    n_global_vars = 0;

    for(int i = 0; i < n_var; i ++) {
        vars[i].n = vars[i].m_node->val;

        if(!vars[i].m_fu) {
            vars[i].m_node->val = -1 - n_global_vars;
            n_global_vars ++;
        } else{
            if(vars[i].n > vars[i].m_fu->var_n_max)
                vars[i].m_fu->var_n_max = vars[i].n;

            if(vars[i].n < vars[i].m_fu->var_n_min)
                vars[i].m_fu->var_n_min = vars[i].n;
        }
    }

    for(int i = 0; i < n_var; i ++) {
        if(vars[i].m_fu) {
            vars[i].n -= vars[i].m_fu->var_n_min;
            vars[i].m_node->val = vars[i].n;
        }
    }

}

int AST :: checkVars() {
    for(int i = 0; i < m_tree.nodes_num; i ++) {
        Node* node = m_tree.nodes + i;

        if(node->type != Node::TYPE_VAR) {
            continue;
        }

        Node* dec_node = searchDeclarationEarlier(node->par, Node::TYPE_VAR, node->data, node->data_len);

        if(node->par->type == Node::TYPE_DEC_VAR ||
           node->par->type == Node::TYPE_DEC_ARG) {
            if(dec_node) {
                printf("[ERROR] redeclaration var '%s'\n", node->data);
                NodeDumb(node);
                return 0;
            } else {
                continue;
            }
        }

        if(!dec_node) {
            printf("[ERROR] var '%s' isn't declared\n", node->data);
            NodeDumb(node);
            return 0;
        }

        node->val = dec_node->val;
    }

    return 1;
}

void AST :: DumbTokens() {
    printf("\n------------------ [DUMB]   start ------------------ \n");

    printf("\ttoken_n=%d\n", token_n);
    printf("\tmax_token_n=%d\n", max_token_n);

    printf("\ttoken[%p]: {\n", token);

    for(int i = 0; i < token_n; i ++) {
        printf("\t\t");token[i].Dumb();
    }

    printf("\t}\n");

    printf("\n------------------ [DUMB]   finish ------------------ \n");
}

int AST :: useTranslator(const char* str) {
    FILE *file = fopen(str, "w");

    fprintf(file, "call fu%d\n", main_n);
    fprintf(file, "end\n\n");

    TreeAssDumb(&m_tree, file);

    fclose(file);
}
