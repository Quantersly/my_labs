#include <stdio.h>

#include "matr.h"


int main() {
    Matrix* rm = NULL;
    Matrix* new_rm = NULL;
    while (1) {
        Err_matr res = input_matrix(&rm);
        if (res == ERR_MATR_EOF) {
            printf("\nВвод окончен!\n");
            break;
        } else if (res == ERR_MATR_MEM) {
            printf("\nОшибка при работе с памятью!\n");
            break;
        }
        printf("\"input\":\n");
        print_matrix(rm);
        printf("\"output\":\n");
        res = ind_matrix(rm, &new_rm);
        if(res == ERR_MATR_MEM) {
            printf("\nОшибка при работе с памятью\n");
            erase_matrix(rm);
            break;
        }
        print_matrix(new_rm);
        erase_matrix(rm);
        erase_matrix(new_rm);
    }
    return 0;
}
