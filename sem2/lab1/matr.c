#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#include "matr.h"
#include "input.h"


Err_matr input_matrix(Matrix **rm) {
    int cnt_lines, cnt_el;
    int *line;

    //Выделяем память для матрицы
    *rm = (Matrix*)calloc(1, sizeof(Matrix));
    if (*rm == NULL) {
        (*rm)->lines = 0;
        (*rm)->matr = NULL;
        erase_matrix(*rm);
        return ERR_MATR_MEM;
    }

    //Выделяем память на объекты Line в матрице
    printf("Введите число линий матрицы: --> ");
    Err_inp res_inp = input_natural_int(&cnt_lines);
    if (res_inp == EOF) {
        (*rm)->lines = 0;
        (*rm)->matr = NULL;
        erase_matrix(*rm);
        return ERR_MATR_EOF;
    }
    (*rm)->lines = cnt_lines;
    (*rm)->matr = (Line*)calloc(cnt_lines, sizeof(Line));
    if ((*rm)->matr == NULL) {
        return ERR_MATR_MEM;
    }

    for (int i = 0; i < (*rm)->lines; i++) {
        printf("Введите число элементов линии %d: -->", i + 1);
        res_inp = input_natural_int(&cnt_el);
        if (res_inp == EOF) {
            (*rm)->lines = i;
            erase_matrix(*rm);
            return ERR_MATR_EOF;
        }
        (*rm)->matr[i].el = cnt_el;
        line = (int*)malloc(sizeof(int*) * cnt_el);
        if (line == NULL) {
            (*rm)->lines = i;
            erase_matrix(*rm);
            return ERR_MATR_MEM;
        }
        (*rm)->matr[i].lin = line;
        //Выделение памяти для массива данных у отдельного объекта Line

        printf("Введите элементы для линии №%d матрицы:\n", i + 1);
        for (int j = 0; j < cnt_el; j++, line++) {
            Err_inp inp_res = input_any_int(line);
            if (inp_res == EOF) {
                (*rm)->lines = i + 1;
                erase_matrix(*rm);
                return ERR_MATR_EOF;
            }
            //Заполениние масива данных у отдельного объекта Line
        }
    }
    return ERR_MATR_OK;
}


void erase_matrix(Matrix *rm) {
    for (int i = 0; i < rm->lines; i++) {
        free(rm->matr[i].lin);
        rm->matr[i].lin = NULL;
    }
    free(rm->matr);
    rm->matr = NULL;
    free(rm);
    rm = NULL;
}


void print_matrix(Matrix *rm) {
    for (int i = 0; i < rm->lines; i++) {
        for (int j = 0; j < rm->matr[i].el; j++) {
            printf("%d, ", rm->matr[i].lin[j]);
        }
        printf("\n");
    }
}


Err_matr ind_matrix(Matrix *rm, Matrix **new_rm) {
    *new_rm = (Matrix*)calloc(1, sizeof(Matrix));
    if (*new_rm == NULL) {
        (*new_rm)->lines = 0;
        (*new_rm)->matr = NULL;
        return ERR_MATR_MEM;
    }
    //Выделение памаяти для результирующей матрицы

    (*new_rm)->matr = (Line*)calloc(rm->lines, sizeof(Line));
    if ((*new_rm)->matr == NULL) {
        return ERR_MATR_MEM;    
    }
    (*new_rm)->lines = rm->lines;
    //Выделение памяти для объектов Line у новой матрицы

    int num = 0;
    for (int i = 0; i < rm->lines; i++) {
        int cnt = 0;
        (*new_rm)->matr[num].lin = (int*)calloc(rm->matr[i].el, sizeof(int));
        if ((*new_rm)->matr[num].lin == NULL) {
            (*new_rm)->lines = 0;
            (*new_rm)->matr = NULL;
            return ERR_MATR_MEM; 
        }
        //Выделение памяти для массива данных у отдельного объекта Line у новой матрицы

        for (int j = 0; j < rm->matr[i].el; j++) {
            int fl = flip_num(rm->matr[i].lin[j]);
            if (fl == rm->matr[i].lin[j]) {
                (*new_rm)->matr[num].lin[cnt++] = fl;
            }
        }
        //Проверка данных массива на палиндромы для новой матрицы

        if (cnt != 0) {
            (*new_rm)->matr[num++].el = cnt;
        } else {
            free((*new_rm)->matr[num].lin);
            (*new_rm)->matr[num].lin = NULL;
            (*new_rm)->matr[num].el = cnt;
            //Освобождение памяти для объекта Line, так как он пустой
        }
    }
    if (num != 0) {
        (*new_rm)->matr = (Line*)realloc((*new_rm)->matr, num * sizeof(Line));
        (*new_rm)->lines = num;
        if ((*new_rm)->matr == NULL) {
            return ERR_MATR_MEM;
        }
        //Переопределяем память для объектов Line для новой матирцы
    } else {
        free((*new_rm)->matr);
        (*new_rm)->matr = NULL;
        (*new_rm)->lines = 0;
        //Освобождаем память для новой матрицы, так как она пустая
    }
        return ERR_MATR_OK;
}


int flip_num(int num) {
    int ans = 0;
    while (num != 0) {
        ans *= 10;
        ans += (num % 10);
        num /= 10;
    }
    return ans;
}

