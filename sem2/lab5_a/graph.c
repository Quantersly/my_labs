#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <limits.h>
#include "graph.h"

#define INF UINT_MAX

Err_graph graph_create(Graph **g, size_t capacity) {
    if (*g) graph_erase(g);
    *g = (Graph*)calloc(1, sizeof(Graph));
    if (!*g) return ERR_GRAPH_MEM;
    (*g)->lists = (AdjList*)calloc(capacity, sizeof(AdjList));
    if (!(*g)->lists) { free(*g); *g = NULL; return ERR_GRAPH_MEM; }
    (*g)->capacity = capacity;
    (*g)->size = 0;
    return ERR_GRAPH_OK;
}

void graph_erase(Graph **g) {
    if (!g || !*g) return;
    for (size_t i = 0; i < (*g)->size; i++) {
        AdjNode *cur = (*g)->lists[i].head;
        while (cur) {
            AdjNode *tmp = cur;
            cur = cur->next;
            free(tmp);
        }
    }
    free((*g)->lists);
    free(*g);
    *g = NULL;
}

int graph_find_vertex(const Graph *g, unsigned int x, unsigned int y) {
    if (!g) return -1;
    for (size_t i = 0; i < g->size; i++) {
        if (g->lists[i].data.x == x && g->lists[i].data.y == y)
            return (int)i;
    }
    return -1;
}

static int are_adjacent_cells(const VertexData *a, const VertexData *b) {
    int dx = (int)a->x - (int)b->x;
    int dy = (int)a->y - (int)b->y;
    return (abs(dx) + abs(dy)) == 1;
}

static int has_edge(const Graph *g, VertexId from, VertexId to) {
    AdjNode *cur = g->lists[from].head;
    while (cur) {
        if (cur->dest == to) return 1;
        cur = cur->next;
    }
    return 0;
}

Err_graph graph_add_vertex(Graph *g, unsigned int x, unsigned int y, CellType type) {
    if (!g) return ERR_GRAPH_INVALID;
    if (g->size >= g->capacity) return ERR_GRAPH_MEM;
    if (graph_find_vertex(g, x, y) >= 0) return ERR_GRAPH_DUPLICATE;
    g->lists[g->size].data.x = x;
    g->lists[g->size].data.y = y;
    g->lists[g->size].data.type = type;
    g->lists[g->size].head = NULL;
    g->size++;
    return ERR_GRAPH_OK;
}

Err_graph graph_add_edge(Graph *g, VertexId v1, VertexId v2) {
    if (!g) return ERR_GRAPH_INVALID;
    if (v1 >= g->size || v2 >= g->size) return ERR_GRAPH_NOT_FOUND;
    if (v1 == v2) return ERR_GRAPH_INVALID;
    if (!are_adjacent_cells(&g->lists[v1].data, &g->lists[v2].data))
        return ERR_GRAPH_NOT_ADJACENT;
    if (has_edge(g, v1, v2)) return ERR_GRAPH_DUPLICATE;

    AdjNode *n1 = (AdjNode*)malloc(sizeof(AdjNode));
    AdjNode *n2 = (AdjNode*)malloc(sizeof(AdjNode));
    if (!n1 || !n2) { free(n1); free(n2); return ERR_GRAPH_MEM; }
    n1->dest = v2; n1->next = g->lists[v1].head; g->lists[v1].head = n1;
    n2->dest = v1; n2->next = g->lists[v2].head; g->lists[v2].head = n2;
    return ERR_GRAPH_OK;
}

Err_graph graph_remove_vertex(Graph *g, VertexId v) {
    if (!g) return ERR_GRAPH_INVALID;
    if (v >= g->size) return ERR_GRAPH_NOT_FOUND;

    for (size_t i = 0; i < g->size; i++) {
        if (i == v) continue;
        AdjNode **cur = &g->lists[i].head;
        while (*cur) {
            if ((*cur)->dest == v) {
                AdjNode *tmp = *cur;
                *cur = (*cur)->next;
                free(tmp);
            } else {
                cur = &(*cur)->next;
            }
        }
    }
    AdjNode *cur = g->lists[v].head;
    while (cur) { AdjNode *tmp = cur; cur = cur->next; free(tmp); }

    for (size_t i = v; i < g->size - 1; i++)
        g->lists[i] = g->lists[i + 1];
    g->size--;

    for (size_t i = 0; i < g->size; i++) {
        AdjNode *node = g->lists[i].head;
        while (node) {
            if (node->dest > v) node->dest--;
            node = node->next;
        }
    }
    return ERR_GRAPH_OK;
}

Err_graph graph_remove_edge(Graph *g, VertexId v1, VertexId v2) {
    if (!g) return ERR_GRAPH_INVALID;
    if (v1 >= g->size || v2 >= g->size) return ERR_GRAPH_NOT_FOUND;

    int found = 0;
    AdjNode **cur = &g->lists[v1].head;
    while (*cur) {
        if ((*cur)->dest == v2) { AdjNode *tmp = *cur; *cur = (*cur)->next; free(tmp); found = 1; break; }
        cur = &(*cur)->next;
    }
    cur = &g->lists[v2].head;
    while (*cur) {
        if ((*cur)->dest == v1) { AdjNode *tmp = *cur; *cur = (*cur)->next; free(tmp); found = 1; break; }
        cur = &(*cur)->next;
    }
    return found ? ERR_GRAPH_OK : ERR_GRAPH_NOT_FOUND;
}

Err_graph graph_change_vertex(Graph *g, VertexId v, unsigned int x, unsigned int y, CellType type) {
    if (!g) return ERR_GRAPH_INVALID;
    if (v >= g->size) return ERR_GRAPH_NOT_FOUND;
    int idx = graph_find_vertex(g, x, y);
    if (idx >= 0 && (VertexId)idx != v) return ERR_GRAPH_DUPLICATE;
    g->lists[v].data.x = x;
    g->lists[v].data.y = y;
    g->lists[v].data.type = type;
    return ERR_GRAPH_OK;
}

void graph_print(const Graph *g) {
    if (!g) { printf("Граф не существует (NULL)\n"); return; }
    if (g->size == 0) { printf("Граф пуст\n"); return; }
    printf("--- Списки смежности (вершин: %zu) ---\n", g->size);
    for (size_t i = 0; i < g->size; i++) {
        const char *tname = g->lists[i].data.type == CELL_ENTRANCE ? "вход" :
                            g->lists[i].data.type == CELL_EXIT ? "выход" : "обычная";
        printf("[%zu] (%u,%u) %s -> ", i, g->lists[i].data.x, g->lists[i].data.y, tname);
        AdjNode *cur = g->lists[i].head;
        while (cur) { printf("%u ", cur->dest); cur = cur->next; }
        printf("\n");
    }
    printf("--------------------------------------\n");
}

/*
Err_graph graph_export_dot(const Graph *g, const char *filename) {
    if (!g) return ERR_GRAPH_INVALID;
    FILE *f = fopen(filename, "w");
    if (!f) return ERR_GRAPH_FILE_ERROR;
    fprintf(f, "graph Labyrinth {\n");
    for (size_t i = 0; i < g->size; i++) {
        const char *color = g->lists[i].data.type == CELL_ENTRANCE ? "green" :
                            g->lists[i].data.type == CELL_EXIT ? "red" : "white";
        fprintf(f, "  %zu [label=\"(%u,%u)\" style=filled fillcolor=%s];\n",
                i, g->lists[i].data.x, g->lists[i].data.y, color);
    }
    for (size_t i = 0; i < g->size; i++) {
        AdjNode *cur = g->lists[i].head;
        while (cur) {
            if (cur->dest > i)
                fprintf(f, "  %zu -- %u;\n", i, cur->dest);
            cur = cur->next;
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    return ERR_GRAPH_OK;
}
*/

Err_graph graph_export_dot(const Graph *g, const char *filename) {
    if (!g) return ERR_GRAPH_INVALID;
    FILE *f = fopen(filename, "w");
    if (!f) return ERR_GRAPH_FILE_ERROR;
    fprintf(f, "graph Labyrinth {\n");
    fprintf(f, "  graph [splines=false layout=neato overlap=false];\n");
    fprintf(f, "  node [shape=square width=0.8 fixedsize=true style=filled];\n");
    fprintf(f, "  edge [penwidth=4];\n");
    for (size_t i = 0; i < g->size; i++) {
        const char *color = g->lists[i].data.type == CELL_ENTRANCE ? "green" :
                            g->lists[i].data.type == CELL_EXIT ? "red" : "lightgray";
        fprintf(f, "  %zu [label=\"\" style=filled fillcolor=%s pos=\"%u,%u!\"];\n",
                i, color, g->lists[i].data.x, g->lists[i].data.y);
    }
    for (size_t i = 0; i < g->size; i++) {
        AdjNode *cur = g->lists[i].head;
        while (cur) {
            if (cur->dest > i)
                fprintf(f, "  %zu -- %u;\n", i, cur->dest);
            cur = cur->next;
        }
    }
    fprintf(f, "}\n");
    fclose(f);
    return ERR_GRAPH_OK;
}

Err_graph graph_dfs_reachable(const Graph *g, VertexId start, int *reachable) {
    if (!g) return ERR_GRAPH_INVALID;
    if (start >= g->size) return ERR_GRAPH_NOT_FOUND;
    if (g->lists[start].data.type != CELL_ENTRANCE) return ERR_GRAPH_INVALID;

    *reachable = 0;
    int *visited = (int*)calloc(g->size, sizeof(int));
    VertexId *stack = (VertexId*)malloc(g->size * sizeof(VertexId));
    if (!visited || !stack) { free(visited); free(stack); return ERR_GRAPH_MEM; }

    size_t top = 0;
    stack[top++] = start;
    visited[start] = 1;

    while (top > 0) {
        VertexId v = stack[--top];
        if (g->lists[v].data.type == CELL_EXIT) {
            *reachable = 1;
            break;
        }
        AdjNode *cur = g->lists[v].head;
        while (cur) {
            if (!visited[cur->dest]) {
                visited[cur->dest] = 1;
                stack[top++] = cur->dest;
            }
            cur = cur->next;
        }
    }
    free(visited);
    free(stack);
    return ERR_GRAPH_OK;
}

Err_graph graph_bellman_ford(const Graph *g, VertexId src, VertexId dst, VertexId **path, size_t *path_len) {
    if (!g) return ERR_GRAPH_INVALID;
    if (src >= g->size || dst >= g->size) return ERR_GRAPH_NOT_FOUND;

    size_t n = g->size;
    unsigned int *dist = (unsigned int*)malloc(n * sizeof(unsigned int));
    int *prev = (int*)malloc(n * sizeof(int));
    if (!dist || !prev) { free(dist); free(prev); return ERR_GRAPH_MEM; }

    for (size_t i = 0; i < n; i++) { dist[i] = INF; prev[i] = -1; }
    dist[src] = 0;

    for (size_t iter = 0; iter < n - 1; iter++) {
        int updated = 0;
        for (size_t u = 0; u < n; u++) {
            if (dist[u] == INF) continue;
            AdjNode *cur = g->lists[u].head;
            while (cur) {
                if (dist[u] + 1 < dist[cur->dest]) {
                    dist[cur->dest] = dist[u] + 1;
                    prev[cur->dest] = (int)u;
                    updated = 1;
                }
                cur = cur->next;
            }
        }
        if (!updated) break;
    }

    if (dist[dst] == INF) {
        free(dist); free(prev);
        return ERR_GRAPH_NO_PATH;
    }

    *path_len = dist[dst] + 1;
    *path = (VertexId*)malloc(*path_len * sizeof(VertexId));
    if (!*path) { free(dist); free(prev); return ERR_GRAPH_MEM; }

    int cur_v = (int)dst;
    for (size_t i = *path_len; i > 0; i--) {
        (*path)[i - 1] = (VertexId)cur_v;
        cur_v = prev[cur_v];
    }

    free(dist);
    free(prev);
    return ERR_GRAPH_OK;
}

Err_graph graph_floyd_nearest_exit(const Graph *g, VertexId entrance, VertexId *nearest_exit, unsigned int *distance) {
    if (!g) return ERR_GRAPH_INVALID;
    if (entrance >= g->size) return ERR_GRAPH_NOT_FOUND;
    if (g->lists[entrance].data.type != CELL_ENTRANCE) return ERR_GRAPH_INVALID;

    size_t n = g->size;
    unsigned int *dist = (unsigned int*)malloc(n * n * sizeof(unsigned int));
    if (!dist) return ERR_GRAPH_MEM;

    #define D(i,j) dist[(i)*n+(j)]

    for (size_t i = 0; i < n; i++)
        for (size_t j = 0; j < n; j++)
            D(i, j) = (i == j) ? 0 : INF;

    for (size_t u = 0; u < n; u++) {
        AdjNode *cur = g->lists[u].head;
        while (cur) {
            D(u, cur->dest) = 1;
            cur = cur->next;
        }
    }

    for (size_t k = 0; k < n; k++)
        for (size_t i = 0; i < n; i++)
            for (size_t j = 0; j < n; j++)
                if (D(i, k) != INF && D(k, j) != INF && D(i, k) + D(k, j) < D(i, j))
                    D(i, j) = D(i, k) + D(k, j);

    *distance = INF;
    *nearest_exit = 0;
    for (size_t i = 0; i < n; i++) {
        if (g->lists[i].data.type == CELL_EXIT && D(entrance, i) < *distance) {
            *distance = D(entrance, i);
            *nearest_exit = (VertexId)i;
        }
    }

    #undef D
    free(dist);

    if (*distance == INF) return ERR_GRAPH_NO_PATH;
    return ERR_GRAPH_OK;
}

Err_graph graph_import(Graph **g, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) return ERR_GRAPH_FILE_ERROR;

    size_t cap;
    if (fscanf(f, "%zu", &cap) != 1) { fclose(f); return ERR_GRAPH_FILE_ERROR; }

    Err_graph res = graph_create(g, cap);
    if (res != ERR_GRAPH_OK) { fclose(f); return res; }

    size_t nv;
    if (fscanf(f, "%zu", &nv) != 1) { fclose(f); graph_erase(g); return ERR_GRAPH_FILE_ERROR; }

    for (size_t i = 0; i < nv; i++) {
        unsigned int x, y, t;
        if (fscanf(f, "%u %u %u", &x, &y, &t) != 3) { fclose(f); graph_erase(g); return ERR_GRAPH_FILE_ERROR; }
        res = graph_add_vertex(*g, x, y, (CellType)t);
        if (res != ERR_GRAPH_OK) { fclose(f); graph_erase(g); return res; }
    }

    size_t ne;
    if (fscanf(f, "%zu", &ne) != 1) { fclose(f); graph_erase(g); return ERR_GRAPH_FILE_ERROR; }

    for (size_t i = 0; i < ne; i++) {
        unsigned int v1, v2;
        if (fscanf(f, "%u %u", &v1, &v2) != 2) { fclose(f); graph_erase(g); return ERR_GRAPH_FILE_ERROR; }
        res = graph_add_edge(*g, (VertexId)v1, (VertexId)v2);
        if (res != ERR_GRAPH_OK) { fclose(f); graph_erase(g); return res; }
    }

    fclose(f);
    return ERR_GRAPH_OK;
}

static void url_encode(const char *src, char *dst, size_t dst_size) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j + 4 < dst_size; i++) {
        unsigned char c = (unsigned char)src[i];
        j += snprintf(dst + j, dst_size - j, "%%%02X", c);
    }
    dst[j] = '\0';
}

Err_graph graph_graphviz(const Graph *g) {
    if (!g) return ERR_GRAPH_INVALID;

    char dot[32768];
    size_t len = snprintf(dot, sizeof(dot),
        "graph Labyrinth {"
        "graph [splines=false layout=neato overlap=false];"
        "node [shape=square width=0.8 fixedsize=true style=filled];"
        "edge [penwidth=4];");

    for (size_t i = 0; i < g->size; i++) {
        const char *color = g->lists[i].data.type == CELL_ENTRANCE ? "green" :
                            g->lists[i].data.type == CELL_EXIT ? "red" : "lightgray";
        len += snprintf(dot + len, sizeof(dot) - len,
            "%zu [fillcolor=%s pos=\"%u,%u!\" width=0.8 height=0.8 fixedsize=true label=\" \"];",
            i, color, g->lists[i].data.x * 100, g->lists[i].data.y * 100);
    }

    for (size_t i = 0; i < g->size; i++) {
        AdjNode *cur = g->lists[i].head;
        while (cur) {
            if (cur->dest > i)
                len += snprintf(dot + len, sizeof(dot) - len,
                    "%zu -- %u;", i, cur->dest);
            cur = cur->next;
        }
    }

    snprintf(dot + len, sizeof(dot) - len, "}");

    char encoded[65536];
    url_encode(dot, encoded, sizeof(encoded));

    printf("\nСсылка на граф (Ctrl+Click):\n");
    printf("https://dotrend.dozen.mephi.ru/?dot=%s\n", encoded);

    return ERR_GRAPH_OK;
}

