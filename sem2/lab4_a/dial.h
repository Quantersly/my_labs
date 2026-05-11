#ifndef DIAL_H
#define DIAL_H

#include "tree.h"

void     dial_exit(void);
Err_tree dial_create(Tree **tree);
Err_tree dial_insert(Tree *tree);
Err_tree dial_delete(Tree *tree);
Err_tree dial_traverse(Tree *tree);
Err_tree dial_search(Tree *tree);
Err_tree dial_search_closest(Tree *tree);
Err_tree dial_import(Tree *tree);
Err_tree dial_graphviz(Tree *tree);

#endif