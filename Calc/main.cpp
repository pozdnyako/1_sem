#include "RDP.h"

int main(int argc, char* argv[]) {
    Calc calc;
    char str[80] = "(2+2)*2-9+1488";

    double res = calc.getG(str);
    printf("%s = %f", str, res);
    return 0;
}
