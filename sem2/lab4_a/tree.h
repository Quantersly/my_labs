#ifndef TREE_H
#define TREE_H

#include <stddef.h>

typedef char *KeyType;
typedef unsigned int InfoType;

typedef enum {
    ERR_TREE_EOF = -1,
    ERR_TREE_OK = 0,
    ERR_TREE_MEM,
    ERR_TREE_NOT_FOUND,
    ERR_TREE_KEY_UPDATED,
    ERR_TREE_FILE_NOT_FOUND,
    ERR_TREE_FILE_INCORRECT,
    ERR_TREE_EMPTY,
    ERR_TREE_VOID_KEY,
} Err_tree;

typedef struct Node {
    KeyType key;
    InfoType *info;

    struct Node *left;
    struct Node *right;
    struct Node *parent;

    struct Node *prev;
} Node;

typedef struct {
    Node *root;
} Tree;

Err_tree tree_create(Tree **tree);
void     tree_free(Tree *tree);

Err_tree tree_insert(Tree *tree, const char *key, InfoType value, InfoType *old_value);
Err_tree tree_delete(Tree *tree, const char *key);

Err_tree tree_search(const Tree *tree, const char *key, Node ***out, size_t *out_size);
Err_tree tree_search_closest(const Tree *tree, const char *key, Node ***out, size_t *out_size);

Err_tree tree_traverse(const Tree *tree, const char *limit, Node ***out, size_t *out_size);

Err_tree tree_import(Tree *tree, const char *filename);
void     tree_print(const Tree *tree);
Err_tree tree_graphviz(const Tree *tree);

#endif