#include "Tree.h"
#include "RDP.h"

int main(int argc, char* argv[]) {
    FILE* texout = fopen("tex.tex", "w");
    fprintf(texout, "\\documentclass[12pt]{article}\n\\begin{document}");

    Tree dTree;
    TreeCtor(&dTree);

    Calc calc;
    calc.getG("(x-5)^(2/3));

    fprintf(texout, "\\[ \\left( ");
    TreeTexDumb(&calc.m_tree, texout);
    fprintf(texout, "\\right) ' = ");

    TreeDumb(&calc.m_tree);
    system("D:\\Users\\Admin\\Downloads\\graphviz-2.38\\release\\bin\\dotty.exe D:\\Users\\Admin\\Documents\\GitHub\\1_sem\\1_sem\\task_6\\out.txt");


    Diff(&dTree, calc.m_tree.root);

    TreeDumb(&dTree);
    system("D:\\Users\\Admin\\Downloads\\graphviz-2.38\\release\\bin\\dotty.exe D:\\Users\\Admin\\Documents\\GitHub\\1_sem\\1_sem\\task_6\\out.txt");
    //TreeInfDumb(&dTree);

    int count = 0;


    do{
        count = 0;
        count += NodeDelete0MultSmth(dTree.root);
        count += NodeDelete1MultSmth(dTree.root);
        count += NodeDelete0PlusSmth(dTree.root);
        count += NodeDelete0DivSmth(dTree.root);
        count += NodeDeleteSmthMin0(dTree.root);

    } while(count > 0);

    TreeDumb(&dTree);
    system("D:\\Users\\Admin\\Downloads\\graphviz-2.38\\release\\bin\\dotty.exe D:\\Users\\Admin\\Documents\\GitHub\\1_sem\\1_sem\\task_6\\out.txt");

    TreeTexDumb(&dTree, texout);
    fprintf(texout, " \\]\n\\end{document}");

    TreeInfDumb(&dTree);

    return 0;
}
