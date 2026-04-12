#include <stdio.h>
#include <math.h>
#include "input.h"

double product(double, int*);

int main() {
    int response = 0, i_member = 0;
    double accuracy = 0, code_result = 0, math_result = 0;
    printf(YEL "Введите точность числа большую 0:\n" RESET);
    response = input_positive_non_zero_double(&accuracy);
    if (response != 2) {
        printf(RED "\nПрограмма завершила свою работу\n" RESET);
        return 0;
    }
    code_result = product(accuracy, &i_member);
    math_result = sqrt(2) / 2;
    printf(GRN "Результат вычислений:    " RESET " %.16lf\n", code_result);
    printf(GRN "Результат через math.h:  " RESET " %.16lf\n", math_result);
    printf(YEL "Количество членов в ряду:" RESET " %d\n", i_member);
    return 0;
}


double product(double accuracy, int *i_member) {
    int sign = 1;
    double code_result = 1, old_result = 0, fraction = 1;
    while (sign * (code_result - old_result) >= accuracy) {
        (*i_member) ++;
        sign *= -1;
        old_result = code_result;
        fraction *= -((*i_member * 2.0 - 1.0) / (*i_member * 2.0 + 1.0));
        code_result *= (1.0 + fraction);
    }
    return code_result;
}

