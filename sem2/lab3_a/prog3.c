#include <string.h>
#include <stdio.h>

#include "input.h"
#include "table.h"
#include "dial.h"

void menu();


int main() {
    char option = ' ';
    Table *my_table = NULL;
    while (option != '0') {
        Err_table res_table = ERR_TABLE_OK;
        menu();
        Table *ans_table = NULL;
        Err_inp res_option = input_any_symbol(&option);
        if (res_option == EOF) {
            printf(RED "Срочное завершение программы!\n" RESET);
            break;
        }
        switch (option) {
            case '0':
                dial_exit();
                break;
            case '1':
                dial_create(&my_table);
                break;
            case '2':
                res_table = dial_insert(my_table);
                break;
            case '3':
                res_table = dial_delete(my_table);
                break;
            case '4':
                res_table = dial_search(my_table, &ans_table);
                table_print(ans_table);
                table_erase(&ans_table);
                break;
            case '5':
                break;
            case '6':
                res_table = dial_import(&my_table);
                break;
            case '7':
                res_table = dial_search_parent(my_table, &ans_table);
                table_print(ans_table);
                table_erase(&ans_table);
                break;
            default:
                printf(RED"\nОшибка ввода!\n"RESET);
        }
        table_print(my_table);
        switch (res_table) {
            case ERR_TABLE_EOF:
                printf(RED "Срочное завершение программы!\n" RESET);
                option = '0';
                break;
            case ERR_TABLE_VOID_NAME:
                printf(RED "Название файла не может быть пустым!\n" RESET);
                break;
            case ERR_TABLE_DUBLICATE_KEY:
                printf(RED "Значение ключей должны быть уникальными!\n" RESET);
                break;
            case ERR_TABLE_NON_POSITIVE_VAR:
                printf(RED "Значения полей должны быть не отрицательными \(в случае ключа - положительными)!\n" RESET);
                break;
            case ERR_TABLE_PARENT_NOT_FOUND:
                printf(RED "Если родительский ключ не равен нулю, то должен уже быть в таблице элемент с таким ключом!\n" RESET);
                break;
            case ERR_TABLE_FULL:
                printf(RED "Таблица заполнена!\n" RESET);
                break;
            case ERR_TABLE_FULL_IMPORT:
                printf(RED "Недостаточный размер таблицы для такого количества данных!\n" RESET);
                break;
            case ERR_TABLE_NOT_FOUND:
                printf(RED "Элементы с таким значением ключа не найдены!\n" RESET);
                break;
            case ERR_TABLE_FILE_NOT_FOUND:
                printf(RED "Файл с таким именем не найден!\n" RESET);
                break;
            case ERR_TABLE_FILE_INCORRECT:
                printf(RED "Файл неправильно заполнен \(в конце не дожно быть пробелов)\n" RESET);
                break;
            case ERR_TABLE_MEM:
                printf(RED "Ошибка при работе с памятью! Немедленное завершение работы!\n" RESET);
                option = '0';
                break;
            default:
                break;
        }
    }
    table_erase(&my_table);
    return 0;
}


void menu() {
    printf("\n");                                                     
    printf("   *********************** МЕНЮ ***********************\n");
    printf("1. Создание таблицы\n");
    printf("2. Вставка нового элемента\n");
    printf("3. Удаление из таблицы по заданному значению ключа\n");
    printf("4. Поиск в таблице элемента по заданному значению ключа\n");
    printf("5. Вывод содержимого таблицы\n");
    printf("6. Импорт данных из текстового файла\n"); 
    printf("7. Поиск в таблице элементов по родительскому ключу\n");
    printf("0. Завершение работы программы\n");
    printf("\n"); 
}

