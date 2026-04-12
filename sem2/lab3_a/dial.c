#include <stdio.h>
#include <string.h>
#include <readline/readline.h>
#include <stdlib.h>

#include "dial.h"
#include "table.h"
#include "input.h"


void dial_exit() {
    printf("Программа завершает свою работу!\n");
}

Err_table dial_insert(Table *my_table) {
    int key, par, info;
    printf("Введите положительный уникальный ключ:\n");
    Err_inp res_inp = input_natural_int(&key);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    printf("Введите неотрицательный родительский ключ:\n");
    res_inp = input_non_negative_int(&par);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    printf("Введите неотрицальное число поля информации:\n");
    res_inp = input_non_negative_int(&info);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_insert(my_table, (KeyType)key, (KeyType)par, (InfoType)info);
    return res_table;
}

Err_table dial_create(Table **my_table) {
    int msize;
    printf("Введите максимальное количество элементов:\n");
    Err_inp res_inp = input_non_negative_int(&msize);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_create(my_table, (IndexType)msize);
    return res_table;
 
}

Err_table dial_delete(Table *my_table) {
    int key;
    printf("Введите положительный ключ удаляемого элемента:\n");
    Err_inp res_inp = input_natural_int(&key);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_delete(my_table, (KeyType)key);
    return res_table;
}

Err_table dial_search(const Table *my_table, Table **ans_table) {
    int key;
    printf("Введите положительный ключ искомого элемента:\n");
    Err_inp res_inp = input_natural_int(&key);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_search(my_table, ans_table, (KeyType)key);
    return res_table;
}

Err_table dial_import(Table **my_table) {
    char *name_file = NULL;
    int msize;
    printf("Введите название файла для импорта:\n");
    name_file = readline("");
    if (name_file == NULL) {
        return ERR_TABLE_EOF;
    }
    if (*name_file == '\0') {
        return ERR_TABLE_VOID_NAME;
    }
    printf("Введите максимальное количество элементов:\n");
    Err_inp res_inp = input_non_negative_int(&msize);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_import(my_table, name_file, (IndexType)msize);
    free(name_file);
    return res_table;
}

Err_table dial_search_parent(const Table *my_table, Table **ans_table) {
    int par;
    printf("Введите положительный родительский ключ искомого\(-ых) элемента:\n");
    Err_inp res_inp = input_non_negative_int(&par);
    if (res_inp == EOF) {
        return ERR_TABLE_EOF;
    }
    Err_table res_table = table_search_parent(my_table, ans_table, (KeyType)par);
    return res_table;
}


