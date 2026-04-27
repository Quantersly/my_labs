#include <stdio.h>
#include <limits.h>
#include <float.h>
#include <readline/readline.h>
#include "input.h"

Err_inp input_int(int *n, int min, int max) {
    int response = 0;
    char symbol = ' ';
    while (response == 0) {
        response = scanf("%d%c", n, &symbol);
        if (response == EOF) {
            break;
        }
        if (symbol != '\n') {
//          scanf("%*[^\n]%*c");
// TODO 
            scanf("%*[^\n]");
        }
        if (response != ERR_INPUT_OK || symbol != '\n' || *n < min || *n > max) {
            printf(RED "\nОшибка ввода\n" RESET);
            symbol = ' ';
            response = 0;
        }
    }
    return response == EOF ? ERR_INPUT_EOF : ERR_INPUT_OK;
}


Err_inp input_any_int(int *n) {
    return input_int(n, INT_MIN, INT_MAX);
}


Err_inp input_natural_int(int *n) {
    return input_int(n, 1, INT_MAX);
}


Err_inp input_non_negative_int(int *n) {
    return input_int(n, 0, INT_MAX);
}


Err_inp input_double(double *n, double min, double max) {
    int response = 0;
    char symbol = ' ';
    while (response == 0) {
        response = scanf("%lf%c", n, &symbol);
        if (response == EOF) {
            break;
        }
        if (symbol != '\n') {
          //scanf("%*[^\n]%*c");
            scanf("%*[^\n]");
        }
        if (response != ERR_INPUT_OK || symbol != '\n' || *n <= min || *n >= max) {
            printf(RED "\nОшибка ввода\n" RESET);
            symbol = ' ';
            response = 0;
        }
    }
    return response == EOF ? ERR_INPUT_EOF : ERR_INPUT_OK;
}


Err_inp input_any_double(double *n) {
    return input_double(n, DBL_MIN, DBL_MAX);
}


Err_inp input_positive_non_zero_double(double *n) {
    return input_double(n, 0.0, DBL_MAX);
}


Err_inp input_any_symbol(char *symbol_1) {
    int response = 0;
    char symbol_2 = ' ';
    while (response == 0) {
        response = scanf(" %c%c", symbol_1, &symbol_2);
        if (response == EOF) {
            break;
        }
        if (symbol_2 != '\n') {
            scanf("%*[^\n]");
            scanf("%*c");
            *symbol_1 = -1;
        }
    }
    return response == EOF ? ERR_INPUT_EOF : ERR_INPUT_OK;
}


Err_inp input_string(char **str) {
    char *line = readline("");
    if (!line)
        return ERR_INPUT_EOF;
    *str = line;
    return ERR_INPUT_OK;
}