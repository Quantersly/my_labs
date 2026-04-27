#ifndef INPUT_H
#define INPUT_H

#include <stdio.h>
#include <stdlib.h>

typedef enum Err_inp {
    ERR_INPUT_OK = 2,
    ERR_INPUT_EOF = EOF
} Err_inp;

#define BLK "\x1B[30m"
#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

#define BK_RED "\e[41m"

Err_inp input_int(int*, int, int);
Err_inp input_double(double*, double, double);
Err_inp input_positive_non_zero_double(double*);
Err_inp input_any_int(int*);
Err_inp input_natural_int(int*);
Err_inp input_non_negative_int(int*);
Err_inp input_any_symbol(char*);

Err_inp input_string(char **str);

#endif
