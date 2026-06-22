#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dial.h"
#include "input.h"

void dial_exit() {
    printf("Программа завершает свою работу!\n");
}

Err_graph dial_create(Graph **g) {
    int cap;
    printf("Введите максимальное количество вершин:\n");
    Err_inp res = input_natural_int(&cap);
    if (res == EOF) return ERR_GRAPH_EOF;
    return graph_create(g, (size_t)cap);
}

Err_graph dial_add_vertex(Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int x, y, t;
    printf("Введите координату X:\n");
    if (input_non_negative_int(&x) == EOF) return ERR_GRAPH_EOF;
    printf("Введите координату Y:\n");
    if (input_non_negative_int(&y) == EOF) return ERR_GRAPH_EOF;
    printf("Введите тип клетки (0-обычная, 1-вход, 2-выход):\n");
    if (input_int(&t, 0, 2) == EOF) return ERR_GRAPH_EOF;
    return graph_add_vertex(g, (unsigned int)x, (unsigned int)y, (CellType)t);
}

Err_graph dial_add_edge(Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v1, v2;
    printf("Введите индекс первой вершины:\n");
    if (input_non_negative_int(&v1) == EOF) return ERR_GRAPH_EOF;
    printf("Введите индекс второй вершины:\n");
    if (input_non_negative_int(&v2) == EOF) return ERR_GRAPH_EOF;
    return graph_add_edge(g, (VertexId)v1, (VertexId)v2);
}

Err_graph dial_remove_vertex(Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v;
    printf("Введите индекс удаляемой вершины:\n");
    if (input_non_negative_int(&v) == EOF) return ERR_GRAPH_EOF;
    return graph_remove_vertex(g, (VertexId)v);
}

Err_graph dial_remove_edge(Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v1, v2;
    printf("Введите индекс первой вершины ребра:\n");
    if (input_non_negative_int(&v1) == EOF) return ERR_GRAPH_EOF;
    printf("Введите индекс второй вершины ребра:\n");
    if (input_non_negative_int(&v2) == EOF) return ERR_GRAPH_EOF;
    return graph_remove_edge(g, (VertexId)v1, (VertexId)v2);
}

Err_graph dial_change_vertex(Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v, x, y, t;
    printf("Введите индекс вершины:\n");
    if (input_non_negative_int(&v) == EOF) return ERR_GRAPH_EOF;
    printf("Введите новую координату X:\n");
    if (input_non_negative_int(&x) == EOF) return ERR_GRAPH_EOF;
    printf("Введите новую координату Y:\n");
    if (input_non_negative_int(&y) == EOF) return ERR_GRAPH_EOF;
    printf("Введите тип клетки (0-обычная, 1-вход, 2-выход):\n");
    if (input_int(&t, 0, 2) == EOF) return ERR_GRAPH_EOF;
    return graph_change_vertex(g, (VertexId)v, (unsigned int)x, (unsigned int)y, (CellType)t);
}

Err_graph dial_dfs(const Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v;
    printf("Введите индекс вершины-входа:\n");
    if (input_non_negative_int(&v) == EOF) return ERR_GRAPH_EOF;
    int reachable = 0;
    Err_graph res = graph_dfs_reachable(g, (VertexId)v, &reachable);
    if (res == ERR_GRAPH_OK) {
        if (reachable)
            printf("Выход достижим из данного входа!\n");
        else
            printf("Выход НЕ достижим из данного входа.\n");
    }
    return res;
}

Err_graph dial_bellman_ford(const Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int src, dst;
    printf("Введите индекс вершины-входа:\n");
    if (input_non_negative_int(&src) == EOF) return ERR_GRAPH_EOF;
    printf("Введите индекс вершины-выхода:\n");
    if (input_non_negative_int(&dst) == EOF) return ERR_GRAPH_EOF;

    VertexId *path = NULL;
    size_t path_len = 0;
    Err_graph res = graph_bellman_ford(g, (VertexId)src, (VertexId)dst, &path, &path_len);
    if (res == ERR_GRAPH_OK) {
        printf("Кратчайший путь (длина %zu): ", path_len - 1);
        for (size_t i = 0; i < path_len; i++)
            printf("%u ", path[i]);
        printf("\n");
        free(path);
    }
    return res;
}

Err_graph dial_floyd(const Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;
    int v;
    printf("Введите индекс вершины-входа:\n");
    if (input_non_negative_int(&v) == EOF) return ERR_GRAPH_EOF;

    VertexId nearest;
    unsigned int distance;
    Err_graph res = graph_floyd_nearest_exit(g, (VertexId)v, &nearest, &distance);
    if (res == ERR_GRAPH_OK) {
        printf("Ближайший выход: вершина %u (%u,%u), расстояние: %u\n",
               nearest, g->lists[nearest].data.x, g->lists[nearest].data.y, distance);
    }
    return res;
}

Err_graph dial_import(Graph **g) {
    char name[256];
    printf("Введите название файла для импорта:\n");
    if (scanf(" %255s", name) == EOF) return ERR_GRAPH_EOF;
    return graph_import(g, name);
}

Err_graph dial_export_dot(const Graph *g) {
    char name[256];
    printf("Введите название файла для экспорта (.dot):\n");
    if (scanf(" %255s", name) == EOF) return ERR_GRAPH_EOF;
    return graph_export_dot(g, name);
}

Err_graph dial_graphviz(const Graph *g) {
    return graph_graphviz(g);
}
