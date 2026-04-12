#include <stdio.h>

#define BASIS 8

int translation(int);

int main() {
    int val = 0, x = '\n', res = 0, p = 0;
    printf("Введите число в десятичной системе счисления: ");
    p = scanf("%d%c", &val, &x);
    if (x != '\n' || p == 0 || p == -1) {
        printf("\nОшибка ввода\n");
        return 0;
    }
    res = translation(val);
    printf("\nПредставление числа в восьмеричной системе счисления: %d\n", res);
    return 0;
}


int translation(int number) {
    int ans = 0;
    int degree = 1;
    while (number != 0) {
        ans += (number % BASIS) * degree;
        number /= BASIS;
        degree *= 10;
    }
    return ans;
}

