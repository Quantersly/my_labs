#include <stdio.h>
#include <ctype.h>

int input();
int input_digit(char, int, int);
int number_processing(int, int);

int main() {
    input();
    return 0;
}

int input() {
    const int INT_MAX = 2147483647; 
    int count_digits = 0, val = 0, digit = 0;
    char symbol = ' ';
    int is_negative = 0, error_input = 0, first_is_zero = 0, overflow = 0;
    scanf("%c", &symbol);
    while (symbol != '\n' && !error_input) {
        digit = input_digit(symbol, count_digits, is_negative);
        if (count_digits == 0 && digit == 0) {
            first_is_zero = 1;
        }
        if (digit == -1) {
            error_input = 1;
        } else if (digit == -2) {
            is_negative = 1;
        } else {
            if (val > ((INT_MAX - digit) / 10)) {
                overflow = 1;
                error_input = 1;
            }
            val *= 10;
            val += digit;
            count_digits ++;
        }
        scanf("%c", &symbol);
    }
    if (count_digits == 0 && !error_input) {
        printf("Число не было введено\n");
        return 0;
    }
    if (count_digits > 1 && first_is_zero) {
        printf("Если число не 0, то первая его цифра не может являться 0\n");
        return 0;
    }
    if (overflow) {
        printf("Введённое число слишком большое\n");
		return 0;
    }
    if (!error_input) {
        if (first_is_zero) {
            printf("0\n");
            return 0;
        }
        number_processing(val, is_negative);
    }
    return 0;
}


int input_digit(char symbol, int count_digits, int is_negative) {
    int digit;
    int is_digit = isdigit(symbol)
    if (symbol == '-') {
        if (!is_negative && count_digits == 0) {
            digit = -2;
        } else {
            digit = -1;
            printf("\"-\" может стоять только перед числом\n");
        }
    } else if (is_digit) {
        digit = (symbol - '0');
    } else {
        digit = -1;
        printf("Символ \"%c\" недопустим при вводе числа\n", symbol);
    }
    return digit;
}


int number_processing(int number, int is_negative) {
    int digit_n = 0, copy_number = 0, count = 0;
    const int BASIS = 8;
    if (is_negative) {
        printf("-");
    }
    copy_number = number;
    while (copy_number != 0) {
        copy_number /= BASIS;
        count ++;
    }
    for (int i=1; i<=count; i++) {
        copy_number = number;        
        for (int j=0; j<(count-i); j++) {
                copy_number /= BASIS;
        }
        digit_n = (copy_number % BASIS);
        printf("%d", digit_n);
    }
    printf("\n");
    return 0;
}

