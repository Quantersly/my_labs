#include <stdio.h>
#include "array.h"
#include "input.h"
#include "dialogue.h"

void menu();

int main() {
    int *array = NULL;
    int len = 0, cap = 0;
    char option = -1;
    Err_arr res_dial = ERR_ARR_UNK;
    do {
        menu();
        Err_inp res_option = input_any_symbol(&option);
        if (res_option == EOF) {
            printf(RED "Срочное завершение программы!\n" RESET);
            break;
        }
        res_dial = ERR_ARR_OK;
        switch (option) {
        case '0':
            exit_dial();
            break;
        case '1':
            int new_len = 0;
            res_dial = init_arr_dial(array, &len, cap, new_len);
            break;
        case '2':
            res_dial = app_el_dial(array, &len, cap);
            break;
        case '3':
            res_dial = del_el_dial(array, &len);
            break;
        case '4':
            int new_a_len = 0;
            int *new_array = NULL;
            res_dial = sum_gen(array, &len, cap, &new_array, &new_a_len);
            print_arr(new_array, new_a_len, cap);
            free(new_array);
            break;
        case '5':
            break;
        case '6':
            res_dial = change_mem_dial(&array, &len, &cap);
            break;
        default: 
            printf(RED "\nОшибка ввода!\n" RESET);
        }
        print_arr(array, len, cap);
        if (res_dial == ERR_ARR_EOF) {
            printf(RED "\nСрочное завершение программы!\n" RESET);
            break;
        }
        if (res_dial == ERR_ARR_MEM_OUT) {
            printf(RED "\nБыло выделено слишком много памяти!\n");
            printf("\nСрочное завершение программы!\n" RESET);
            break;
        }
        if (res_dial == ERR_ARR_MEM_SHR) {
            printf(RED "\nПамяти для массива недостаточно, операция не была выполнена!\n" RESET);
        }
        if (res_dial == ERR_ARR_MEM_NEG) {
            printf(RED "\nНельзя выделить отрицательное количество памяти\n" RESET);
        }
        if (res_dial == ERR_ARR_IND) {
            printf(RED "\nБыл введён некорректный индекс!\n" RESET);
        }
        if (res_dial == ERR_ARR_NULL) {
            printf(BLK BK_RED "\nПрограммная ошибка на стороне разработчика! Проверьте код!\n" RESET);
            break;
        }
        if (res_dial == ERR_ARR_UNK) {
            printf(RED "\nНеизвестная ошибка! Программа завершает свою работу!\n" RESET);
            break;
        }
    } while (option != '0');
    free(array);
    return 0;
}


void menu() {
        printf("\n"); 
        printf("   ****************** МЕНЮ ******************\n"); 
        printf("1. Инициализация массива\n");
        printf("2. Вставка нового элементав массив по индексу\n");
        printf("3. Удаление элемента массива по индексу\n");
        printf("4. Обработака данных по \"алгоритму\"\n");
        printf("5. Вывод текущего состояния массива\n");
        printf("6. Изменение памяти в массиве\n");
        printf("0. Завершение работы программы\n");
        printf("\n");
}



