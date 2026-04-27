#include <stdio.h>
#include <string.h>

#include "input.h"
#include "tree.h"
#include "dial.h"

void menu(void);

int main(void) {
    char option = ' ';
    Tree *my_tree = NULL;

    while (option != '0') {
        Err_tree res = ERR_TREE_OK;
        menu();

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
                dial_create(&my_tree);
                break;
            case '2':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_insert(my_tree);
                break;
            case '3':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_delete(my_tree);
                break;
            case '4':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_traverse(my_tree);
                break;
            case '5':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_search(my_tree);
                break;
            case '6':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_search_closest(my_tree);
                break;
            case '7':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_import(my_tree);
                break;
            case '8':
                tree_print(my_tree);
                break;
            case '9':
                if (!my_tree)
                    printf(RED "Дерево не создано!\n" RESET);
                else
                    res = dial_graphviz(my_tree);
                break;
            default:
                printf(RED "\nОшибка ввода!\n" RESET);
        }

        switch (res) {
            case ERR_TREE_EOF:
                printf(RED "Срочное завершение программы!\n" RESET);
                option = '0';
                break;
            case ERR_TREE_MEM:
                printf(RED "Ошибка при работе с памятью! Завершение работы!\n" RESET);
                option = '0';
                break;
            case ERR_TREE_NOT_FOUND:
                printf(RED "Элемент с таким ключом не найден!\n" RESET);
                break;
            case ERR_TREE_KEY_UPDATED:
                printf(YEL "Ключ уже существовал — значение обновлено!\n" RESET);
                break;
            case ERR_TREE_FILE_NOT_FOUND:
                printf(RED "Файл с таким именем не найден!\n" RESET);
                break;
            case ERR_TREE_FILE_INCORRECT:
                printf(RED "Файл заполнен некорректно!\n" RESET);
                break;
            case ERR_TREE_EMPTY:
                printf(RED "Дерево пусто!\n" RESET);
                break;
            case ERR_TREE_VOID_KEY:
                printf(RED "Ключ не может быть пустым!\n" RESET);
                break;
            default:
                break;
        }
    }

    tree_free(my_tree);
    return 0;
}

void menu(void) {
    printf("\n");
    printf("   *********************** МЕНЮ ***********************\n");
    printf("1. Создание дерева\n");
    printf("2. Вставка нового элемента\n");
    printf("3. Удаление элемента по ключу\n");
    printf("4. Обход дерева (обратный порядок, до заданного ключа)\n");
    printf("5. Поиск элемента по ключу\n");
    printf("6. Поиск ближайшего по ключу элемента\n");
    printf("7. Загрузка дерева из файла\n");
    printf("8. Вывод дерева\n");
    printf("9. Графический вывод (graphviz)\n");
    printf("0. Завершение работы\n");
    printf("\n");
}