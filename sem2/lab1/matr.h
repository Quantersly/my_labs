#ifndef MATR_H
#define MATR_H

typedef enum Err_matr {
    ERR_MATR_OK = 0,
    ERR_MATR_EOF = EOF,
    ERR_MATR_MEM = -2
} Err_matr;


typedef struct Line {
    int el;
    int *lin;
} Line;


typedef struct Matrix {
    int lines;
    Line *matr;
} Matrix;


Err_matr input_matrix(Matrix**);
void erase_matrix(Matrix*);
void print_matrix(Matrix*);
Err_matr ind_matrix(Matrix*, Matrix**);
int flip_num(int);

#endif
