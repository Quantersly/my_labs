#include "array.h"
#include "input.h"

Err_arr init_arr(int* const array, int* const len, const int cap, int new_len) {
    if (len == NULL) {
        return ERR_ARR_NULL;
    }
    if (new_len > cap) {
        return ERR_ARR_MEM_SHR;
    }
    *len = new_len;
    int element = 0;
    for (int i = 0; i < *len; i++) {
        printf("array[%d] = ", i);
        Err_inp res_inp = input_any_int(&element);
        if (res_inp == EOF) {
            return ERR_ARR_EOF;
        }
        *(array + i) = element;
    }
    return ERR_ARR_OK;
}


Err_arr app_el(int* const array, int* const len, const int cap, const int ind, const int val) {
    if (len == NULL) {
        return ERR_ARR_NULL;
    }
    if (*len == cap) {
        return ERR_ARR_MEM_SHR;
    }
    if (ind < 0) {
        return ERR_ARR_IND;
    }
    for (int i = *len; i > ind; i--) {
        *(array + i) = *(array + i - 1);
    }
    if (ind > *len) {
        *(array + (*len)) = val;
    } else {
        *(array + ind) = val;
    }
    (*len)++;
    return ERR_ARR_OK;
}


Err_arr del_el(int* const array, int* const len, int ind) {
    if (len == NULL) {
        return ERR_ARR_NULL;
    }
    if (ind < 0 || ind >= *len) {
        return ERR_ARR_IND;
    }
    while (ind + 1 < *len) {
        *(array + ind) = *(array + ind + 1);
        ind++;
    }
    (*len)--;
    return ERR_ARR_OK;
}


void print_arr(int* const array, const int len, const int cap) {
    if (len == 0) {
        printf("\nМассив пустой");
    }
    printf("\n");
    for (int i = 0; i < len; i++) {
        printf("array[%d] = %d\n", i, *(array + i));
    }
    printf("Выделено памяти на " YEL "%d" RESET " ячеек и занято " YEL "%d" RESET " элементами\n", cap, len);
}


Err_arr change_mem(int** const array, int* const len, int* const cap, const int new_mem) {
    if (array == NULL || cap == NULL) {
        return ERR_ARR_NULL;
    }
    if (new_mem < 0) {
        return ERR_ARR_MEM_NEG;
    }
    if (new_mem < *len) {
        *len = new_mem;
    }
    *cap = new_mem;
    if ((*cap) == 0) {
        free(*array);
        *array = NULL;
        return ERR_ARR_OK;
    }
    int *new_array = (int*) realloc(*array, *cap * sizeof(int));
    if (new_array == NULL) {
        return ERR_ARR_MEM_OUT;
    }
    *array = new_array;
    return ERR_ARR_OK;
}


Err_arr sum_gen(int* const array, int* const len, const int cap, int** new_array, int* const new_len) {
    int ind = 0, count = 0;
    *new_array = (int*) calloc(cap, sizeof(int));
    while (ind < *len) {
        int sum = search_sum(array, ind, *len, &count);
        if (*(array + ind) == sum) {
            for (int i = 0; i < count; i++) {
                Err_arr res = app_el(*new_array, new_len, cap, *new_len, *(array + ind + 1));
                if (res != ERR_ARR_OK) {
                    return res;
                }
                res = del_el(array, len, ind + 1);
                if (res != ERR_ARR_OK) {
                    return res;
                }
            }        
        }
        ind ++;
    }
    return ERR_ARR_OK;
}


int search_sum(int* const array, const int ind, const int len, int* const count) {
    int sum = 0;
    *count = 0;
    while (((*(array + ind)) > sum) && (ind + (*count) + 1 < len)) {
        (*count)++;
        sum += (*(array + ind + (*count)));
    }
    return sum;
}

