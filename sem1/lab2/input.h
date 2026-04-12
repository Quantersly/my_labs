#ifndef HEADER_FILE
#define HEADER_FILE

#include <stdio.h>

typedef enum Err_inp {
    ERR_INPUT_OK = 2,
    ERR_INPUT_EOF = EOF
} Err_inp;

#define RED "\x1B[31m"
#define GRN "\x1B[32m"
#define YEL "\x1B[33m"
#define BLU "\x1B[34m"
#define MAG "\x1B[35m"
#define CYN "\x1B[36m"
#define WHT "\x1B[37m"
#define RESET "\x1B[0m"

Err_inp input_int(int*, int, int);
Err_inp input_double(double*, double, double);
Err_inp input_positive_non_zero_double(double*);
Err_inp input_natural_int(int*);

#endif
