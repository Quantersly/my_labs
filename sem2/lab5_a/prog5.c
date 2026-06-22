#include <stdio.h>

#include "input.h"
#include "graph.h"
#include "dial.h"

void menu();

int main() {
    char option = ' ';
    Graph *my_graph = NULL;
    while (option != '0') {
        Err_graph res_graph = ERR_GRAPH_OK;
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
                res_graph = dial_create(&my_graph);
                break;
            case '2':
                res_graph = dial_add_vertex(my_graph);
                break;
            case '3':
                res_graph = dial_add_edge(my_graph);
                break;
            case '4':
                res_graph = dial_remove_vertex(my_graph);
                break;
            case '5':
                res_graph = dial_remove_edge(my_graph);
                break;
            case '6':
                res_graph = dial_change_vertex(my_graph);
                break;
            case '7':
                graph_print(my_graph);
                break;
            case '8':
                res_graph = dial_export_dot(my_graph);
                break;
            case '9':
                res_graph = dial_dfs(my_graph);
                break;
            case 'a':
                res_graph = dial_bellman_ford(my_graph);
                break;
            case 'b':
                res_graph = dial_floyd(my_graph);
                break;
            case 'c':
                res_graph = dial_import(&my_graph);
                break;
            case 'd':
                res_graph = dial_graphviz(my_graph);
                break;
            default:
                printf(RED "\nОшибка ввода!\n" RESET);
        }
        switch (res_graph) {
            case ERR_GRAPH_EOF:
                printf(RED "Срочное завершение программы!\n" RESET);
                option = '0';
                break;
            case ERR_GRAPH_MEM:
                printf(RED "Ошибка при работе с памятью!\n" RESET);
                option = '0';
                break;
            case ERR_GRAPH_NOT_FOUND:
                printf(RED "Вершина или ребро не найдены!\n" RESET);
                break;
            case ERR_GRAPH_DUPLICATE:
                printf(RED "Такой элемент уже существует!\n" RESET);
                break;
            case ERR_GRAPH_NOT_ADJACENT:
                printf(RED "Вершины не являются соседними клетками!\n" RESET);
                break;
            case ERR_GRAPH_INVALID:
                printf(RED "Некорректные данные или граф не создан!\n" RESET);
                break;
            case ERR_GRAPH_NO_PATH:
                printf(RED "Путь не найден!\n" RESET);
                break;
            case ERR_GRAPH_FILE_ERROR:
                printf(RED "Ошибка при работе с файлом!\n" RESET);
                break;
            default:
                break;
        }
    }
    graph_erase(&my_graph);
    return 0;
}

void menu() {
    printf("\n");
    printf("   ************** МЕНЮ **************\n");
    printf("1. Создание графа\n");
    printf("2. Добавление вершины\n");
    printf("3. Добавление ребра\n");
    printf("4. Удаление вершины\n");
    printf("5. Удаление ребра\n");
    printf("6. Изменение данных вершины\n");
    printf("7. Вывод графа (списки смежности)\n");
    printf("8. Экспорт в DOT (graphviz)\n");
    printf("9. Обход: достижимость выхода (DFS)\n");
    printf("a. Кратчайший путь (Беллман-Форд)\n");
    printf("b. Ближайший выход (Флойд-Уоршелл)\n");
    printf("c. Импорт графа из файла\n");
    printf("d. ссылка на graphviz\n");
    printf("0. Завершение работы\n");
    printf("\n");
}
