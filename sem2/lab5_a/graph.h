#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>

typedef unsigned int VertexId;

typedef enum {
    CELL_NORMAL = 0,
    CELL_ENTRANCE,
    CELL_EXIT
} CellType;

typedef struct {
    unsigned int x;
    unsigned int y;
    CellType type;
} VertexData;

typedef struct AdjNode {
    VertexId dest;
    struct AdjNode *next;
} AdjNode;

typedef struct {
    VertexData data;
    AdjNode *head;
} AdjList;

typedef struct {
    AdjList *lists;
    size_t capacity;
    size_t size;
} Graph;

typedef enum {
    ERR_GRAPH_EOF = -1,
    ERR_GRAPH_OK = 0,
    ERR_GRAPH_MEM,
    ERR_GRAPH_NOT_FOUND,
    ERR_GRAPH_DUPLICATE,
    ERR_GRAPH_NOT_ADJACENT,
    ERR_GRAPH_INVALID,
    ERR_GRAPH_NO_PATH,
    ERR_GRAPH_FILE_ERROR
} Err_graph;

Err_graph graph_create(Graph**, size_t);
void graph_erase(Graph**);

Err_graph graph_add_vertex(Graph*, unsigned int, unsigned int, CellType);
Err_graph graph_add_edge(Graph*, VertexId, VertexId);
Err_graph graph_remove_vertex(Graph*, VertexId);
Err_graph graph_remove_edge(Graph*, VertexId, VertexId);
Err_graph graph_change_vertex(Graph*, VertexId, unsigned int, unsigned int, CellType);

void graph_print(const Graph*);
Err_graph graph_export_dot(const Graph*, const char*);

Err_graph graph_dfs_reachable(const Graph*, VertexId, int*);
Err_graph graph_bellman_ford(const Graph*, VertexId, VertexId, VertexId**, size_t*);
Err_graph graph_floyd_nearest_exit(const Graph*, VertexId, VertexId*, unsigned int*);

Err_graph graph_import(Graph**, const char*);
Err_graph graph_graphviz(const Graph*);

int graph_find_vertex(const Graph*, unsigned int, unsigned int);

#endif
