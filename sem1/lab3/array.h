#ifndef ARRAY_H
#define ARRAY_H

#include <stdlib.h>
#include <stdio.h>

typedef enum Err_arr {
    ERR_ARR_OK = 0,
    ERR_ARR_EOF = EOF,
    ERR_ARR_MEM_OUT = -2,
    ERR_ARR_MEM_SHR = -3,
    ERR_ARR_MEM_NEG = -4,
    ERR_ARR_IND = -5,
    ERR_ARR_NULL = -6,
    ERR_ARR_UNK = -7
} Err_arr;

Err_arr init_arr(int* const, int* const, const int, int);
Err_arr app_el(int* const, int* const, const int, const int, const int);
Err_arr del_el(int* const, int* const, int);
void print_arr(int* const, const int, const int);
Err_arr change_mem(int** const, int* const, int* const, const int);
Err_arr sum_gen(int* const, int* const, const int, int** const, int* const);
int search_sum(int* const, const int, const int, int* const);

#endif
