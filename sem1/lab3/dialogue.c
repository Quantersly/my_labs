#include <stdio.h>
#include <limits.h>
#include "array.h"
#include "input.h"

void exit_dial() {
   printf("Программа завершает свою работу\n"); 
}


Err_arr init_arr_dial(int* const array, int* const len, const int cap, int new_len) {
    printf("\nВведите количество элементов для записи в массив, большее нуля:\n");
    Err_inp res_inp = input_natural_int(&new_len);
    if (res_inp == EOF) {
        return ERR_ARR_EOF;
    }
    Err_arr res = init_arr(array, len, cap, new_len);
    if (res == ERR_ARR_OK) {
        printf("\nБыло записано " YEL "%d " RESET "элемента(-ов)\n", *len);
        return ERR_ARR_OK;
    }
    return res;
}


Err_arr app_el_dial(int* const array, int* const len, const int cap) {
    int ind = 0;
    printf("\nВведите индекс добавляемого элемента и его значение:\n");
    Err_inp res_inp = input_any_int(&ind);
    if (res_inp == EOF) {
        return ERR_ARR_EOF;
    }
    int val = 0;
    res_inp = input_any_int(&val);
    if (res_inp == EOF) {
        return ERR_ARR_EOF;
    }
    Err_arr res = app_el(array, len, cap, ind, val);
    if (res == ERR_ARR_OK) {
        printf("\nЭлемент был успешно записан\n");
        return ERR_ARR_OK;
    }
    return res;
}


Err_arr del_el_dial(int* const array, int* const len) {
    int ind = 0;
    printf("\nВведите индекс удаляемого элемента:\n");
    Err_inp res_inp = input_any_int(&ind);
    if (res_inp == EOF) {
        return ERR_ARR_EOF;
    }
    Err_arr res = del_el(array, len, ind);
    return res;
}


Err_arr change_mem_dial(int** const array, int* const len, int* const cap) {
    printf("\nСейчас у вас выделено памяти под " YEL "%d " RESET, *cap);
    printf("элементов массива и задано " YEL "%d " RESET "элементов в массиве\n", *len);
    printf("Введите, на какое число элементов, изменить память:\n");
    int new_mem = 0;
    Err_inp res_inp = input_any_int(&new_mem);
    if (res_inp == EOF) {
        return ERR_ARR_EOF;
    }
    Err_arr res = change_mem(array, len, cap, new_mem);
    if (res == ERR_ARR_OK) {
        printf("\nТеперь выделено память под " YEL "%d " RESET "элементов\n", *cap);
        return ERR_ARR_OK;
    }
    return res;
}

