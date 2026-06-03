#ifndef DIAL_H
#define DIAL_H

#include "graph.h"

void dial_exit();
Err_graph dial_create(Graph**);
Err_graph dial_add_vertex(Graph*);
Err_graph dial_add_edge(Graph*);
Err_graph dial_remove_vertex(Graph*);
Err_graph dial_remove_edge(Graph*);
Err_graph dial_change_vertex(Graph*);
Err_graph dial_dfs(const Graph*);
Err_graph dial_bellman_ford(const Graph*);
Err_graph dial_floyd(const Graph*);
Err_graph dial_import(Graph**);
Err_graph dial_export_dot(const Graph*);
Err_graph dial_graphviz(const Graph*);

#endif
