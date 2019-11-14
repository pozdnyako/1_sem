#include "Tree.h"
#include "RDP.h"
#include "AST.h"

int main(int argc, char* argv[]) {
    setlocale(LC_ALL, "Russian");

    AST ast = AST();
    ast.initSource("src\\sqr.txt");

    ast.useLexer();
    ast.DumbTokens();

    ast.useParser();

    ast.useChecker();

    TreeDumb(&ast.m_tree);
    system("D:\\Users\\Admin\\Downloads\\graphviz-2.38\\release\\bin\\dotty.exe D:\\Users\\Admin\\Documents\\GitHub\\1_sem\\1_sem\\task_9\\out.txt");

    ast.useTranslator("..\\task_4\\src\\code.pyas");
    //system("D:\\Users\\Admin\\Documents\\GitHub\\1_sem\\1_sem\\task_4\\2.exe");
    return 0;
}
