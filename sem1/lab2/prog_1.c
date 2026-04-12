#include <stdio.h>
#include <math.h>
#include "input.h"

double product(int);

int main() {
    int n = 0, response = 0;
    double code_result = 0, math_result = 0;
    printf(YEL "Введите количество членов ряда от 1:\n" RESET);
    response = input_natural_int(&n);
    if (response == EOF) {
        printf(RED "\nПрограмма завершила свою работу\n" RESET);
        return 0;
    }
    code_result = product(n);
    math_result = sqrt(2) / 2;
    printf(GRN "Результат вычислений:  " RESET "%.16lf\n", code_result);
    printf(GRN "Результат через math.h:" RESET "%.16lf\n", math_result);
    return 0;
}


double product(int n) {
    double code_result = 1, fraction = 1;
    for (int i = 1; i <= n; i++) {
        fraction *= -((i * 2.0 - 1.0) / (i * 2.0 + 1.0));
        code_result *= (1.0 + fraction);
    }
    return code_result;
}

