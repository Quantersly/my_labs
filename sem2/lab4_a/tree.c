#define _POSIX_C_SOURCE 200809L

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "tree.h"

// 1. Создание/уничтожение

Err_tree tree_create(Tree **tree) {
    *tree = malloc(sizeof(Tree));
    if (!*tree)
        return ERR_TREE_MEM;

    (*tree)->root = NULL;
    (*tree)->size = 0;

    return ERR_TREE_OK;
}

static Node *node_create(const char *key, InfoType value) {
    Node *node = malloc(sizeof(Node));
    if (!node)
        return NULL;

    node->key = malloc(strlen(key) + 1);
    if (!node->key) {
        free(node);
        return NULL;
    }
    strcpy(node->key, key);

    node->info = malloc(sizeof(InfoType));
    if (!node->info) {
        free(node->key);
        free(node);
        return NULL;
    }
    *(node->info) = value;

    node->left = NULL;
    node->right = NULL;
    node->parent = NULL;
    node->next = NULL;
    node->prev = NULL;

    return node;
}

static void node_free(Node *node) {
    if (!node)
        return;
    node_free(node->left);
    node_free(node->right);
    free(node->key);
    free(node->info);
    free(node);
}

void tree_free(Tree *tree) {
    if (!tree)
        return;
    node_free(tree->root);
    free(tree);
}

// 2. Вспомогательные

static Node *tree_min_node(Node *node) {
    while (node->left)
        node = node->left;
    return node;
}

static void transplant(Tree *tree, Node *u, Node *v) {
    if (!u->parent)
        tree->root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

static void fix_threads(Node *node) {
    if (node->right) {
        Node *cur = node->right;
        while (cur->left)
            cur = cur->left;
        node->next = cur;
    } else {
        Node *cur = node->parent;
        Node *child = node;
        while (cur && child == cur->right) {
            child = cur;
            cur = cur->parent;
        }
        node->next = cur;
    }

    if (node->left) {
        Node *cur = node->left;
        while (cur->right)
            cur = cur->right;
        node->prev = cur;
    } else {
        Node *cur = node->parent;
        Node *child = node;
        while (cur && child == cur->left) {
            child = cur;
            cur = cur->parent;
        }
        node->prev = cur;
    }

    if (node->next)
        node->next->prev = node;
    if (node->prev)
        node->prev->next = node;
}

static void fix_threads_delete(Node *node) {
    if (node->prev)
        node->prev->next = node->next;
    if (node->next)
        node->next->prev = node->prev;
}

// 3. Основные операции

Err_tree tree_insert(Tree *tree, const char *key, InfoType value, InfoType *old_value) {
    if (!key || strlen(key) == 0)
        return ERR_TREE_VOID_KEY;

    if (!tree->root) {
        Node *node = node_create(key, value);
        if (!node)
            return ERR_TREE_MEM;
        tree->root = node;
        tree->size++;
        return ERR_TREE_OK;
    }

    Node *cur = tree->root;
    Node *parent = NULL;

    while (cur) {
        int cmp = strcmp(key, cur->key);
        parent = cur;
        if (cmp == 0) {
            if (old_value)
                *old_value = *(cur->info);
            *(cur->info) = value;
            return ERR_TREE_KEY_UPDATED;
        } else if (cmp < 0) {
            cur = cur->left;
        } else {
            cur = cur->right;
        }
    }

    Node *node = node_create(key, value);
    if (!node)
        return ERR_TREE_MEM;

    node->parent = parent;

    if (strcmp(key, parent->key) < 0)
        parent->left = node;
    else
        parent->right = node;

    fix_threads(node);
    tree->size++;

    return ERR_TREE_OK;
}

Err_tree tree_delete(Tree *tree, const char *key) {
    if (!key || strlen(key) == 0)
        return ERR_TREE_VOID_KEY;
    if (!tree->root)
        return ERR_TREE_EMPTY;

    Node *cur = tree->root;
    while (cur) {
        int cmp = strcmp(key, cur->key);
        if (cmp == 0)
            break;
        cur = cmp < 0 ? cur->left : cur->right;
    }

    if (!cur)
        return ERR_TREE_NOT_FOUND;

    fix_threads_delete(cur);

    if (!cur->left) {
        transplant(tree, cur, cur->right);
    } else if (!cur->right) {
        transplant(tree, cur, cur->left);
    } else {
        Node *successor = tree_min_node(cur->right);
        if (successor->parent != cur) {
            transplant(tree, successor, successor->right);
            successor->right = cur->right;
            successor->right->parent = successor;
        }
        transplant(tree, cur, successor);
        successor->left = cur->left;
        successor->left->parent = successor;
        fix_threads(successor);
    }

    free(cur->key);
    free(cur->info);
    free(cur);
    tree->size--;

    return ERR_TREE_OK;
}


Err_tree tree_search(const Tree *tree, const char *key, Node ***out, size_t *out_size) {
    if (!key || strlen(key) == 0)
        return ERR_TREE_VOID_KEY;
    if (!tree->root)
        return ERR_TREE_EMPTY;

    Node *cur = tree->root;
    while (cur) {
        int cmp = strcmp(key, cur->key);
        if (cmp == 0) {
            *out = malloc(sizeof(Node *));
            if (!*out)
                return ERR_TREE_MEM;
            (*out)[0] = cur;
            *out_size = 1;
            return ERR_TREE_OK;
        }
        cur = cmp < 0 ? cur->left : cur->right;
    }

    return ERR_TREE_NOT_FOUND;
}

Err_tree tree_search_closest(const Tree *tree, const char *key, Node ***out, size_t *out_size) {
    if (!key || strlen(key) == 0)
        return ERR_TREE_VOID_KEY;
    if (!tree->root)
        return ERR_TREE_EMPTY;

    Node *predecessor = NULL;
    Node *successor = NULL;
    Node *cur = tree->root;

    while (cur) {
        int cmp = strcmp(key, cur->key);
        if (cmp == 0) {
            predecessor = cur->prev;
            successor = cur->next;
            break;
        } else if (cmp > 0) {
            predecessor = cur;
            cur = cur->right;
        } else {
            successor = cur;
            cur = cur->left;
        }
    }

    if (!predecessor && !successor)
        return ERR_TREE_NOT_FOUND;

    int diff_pred = predecessor ? strcmp(key, predecessor->key) : -1;
    int diff_succ = successor ? strcmp(successor->key, key) : -1;

    if (predecessor && successor && diff_pred == diff_succ) {
        *out = malloc(2 * sizeof(Node *));
        if (!*out)
            return ERR_TREE_MEM;
        (*out)[0] = predecessor;
        (*out)[1] = successor;
        *out_size = 2;
    } else {
        *out = malloc(sizeof(Node *));
        if (!*out)
            return ERR_TREE_MEM;
        if (!predecessor || (successor && diff_succ < diff_pred))
            (*out)[0] = successor;
        else
            (*out)[0] = predecessor;
        *out_size = 1;
    }

    return ERR_TREE_OK;
}

Err_tree tree_traverse(const Tree *tree, const char *limit, Node ***out, size_t *out_size) {
    if (!tree->root)
        return ERR_TREE_EMPTY;

    size_t count = 0;
    Node *cur = tree_min_node(tree->root);
    while (cur) {
        if (!limit || strcmp(cur->key, limit) <= 0)
            count++;
        cur = cur->next;
    } // избавиться от линейной зависимости

    if (count == 0)
        return ERR_TREE_NOT_FOUND;

    *out = malloc(count * sizeof(Node *));
    if (!*out)
        return ERR_TREE_MEM;

    size_t i = 0;
    cur = tree_min_node(tree->root);
    while (cur && i < count) {
        if (!limit || strcmp(cur->key, limit) <= 0)
            (*out)[i++] = cur;
        cur = cur->next;
    }

    for (size_t l = 0, r = count - 1; l < r; l++, r--) {
        Node *tmp = (*out)[l];
        (*out)[l] = (*out)[r];
        (*out)[r] = tmp;
    }

    *out_size = count;
    return ERR_TREE_OK;
}

// 4. Ввод/вывод 
// избавиться от рекурсии, добавить взамен цикл while для подсчёта глубины через родителя

Err_tree tree_import(Tree *tree, const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f)
        return ERR_TREE_FILE_NOT_FOUND;

    char key[1024];
    unsigned int value;
    Err_tree err = ERR_TREE_OK;

    while (fscanf(f, " %1023[^\n]", key) == 1) {
        size_t len = strlen(key);
        if (len > 0 && key[len - 1] == '\r')
            key[len - 1] = '\0';

        if (fscanf(f, " %u", &value) != 1) {
            err = ERR_TREE_FILE_INCORRECT;
            break;
        }
        InfoType old = 0;
        Err_tree ins = tree_insert(tree, key, value, &old);
        if (ins == ERR_TREE_MEM) {
            err = ERR_TREE_MEM;
            break;
        }
    }

    fclose(f);
    return err;
}

static void print_recursive(const Node *node, int depth, int is_right) {
    if (!node)
        return;
    print_recursive(node->right, depth + 1, 1);
    for (int i = 0; i < depth; i++)
        printf("    ");
    if (depth > 0)
        printf(is_right ? "┌── " : "└── ");
    printf("%s: %u\n", node->key, *(node->info));
    print_recursive(node->left, depth + 1, 0);
}

void tree_print(const Tree *tree) {
    if (!tree || !tree->root) {
        printf("Дерево пусто.\n");
        return;
    }
    printf("\n=== Дерево ===\n");
    print_recursive(tree->root, 0, 0);
    printf("==============\n");
}


static void url_encode(const char *src, char *dst, size_t dst_size) {
    size_t j = 0;
    for (size_t i = 0; src[i] && j + 4 < dst_size; i++) {
        unsigned char c = (unsigned char)src[i];
        j += snprintf(dst + j, dst_size - j, "%%%02X", c);
    }
    dst[j] = '\0';
}

static void graphviz_recursive(const Node *node, FILE *f) {
    if (!node)
        return;
    fprintf(f, "n%s[label=\"{<k>%s|%u|{<l>|<r>}}\"];", node->key, node->key, *(node->info));
    if (node->left) {
        fprintf(f, "n%s:l -> n%s;", node->key, node->left->key);
        graphviz_recursive(node->left, f);
    }
    if (node->right) {
        fprintf(f, "n%s:r -> n%s;", node->key, node->right->key);
        graphviz_recursive(node->right, f);
    }
    // Добавить укзание на узел prev
}

Err_tree tree_graphviz(const Tree *tree) {
    if (!tree || !tree->root)
        return ERR_TREE_EMPTY;

    char dot[16384] = "digraph BST {node[shape=record];";
    size_t len = strlen(dot);

    FILE *f = fmemopen(dot + len, sizeof(dot) - len - 2, "w");
    if (!f)
        return ERR_TREE_MEM;

    graphviz_recursive(tree->root, f);
    fclose(f);

    strncat(dot, "}", sizeof(dot) - strlen(dot) - 1);

    char encoded[65536];
    url_encode(dot, encoded, sizeof(encoded));

    printf("\nСсылка на граф (Ctrl+Click):\n");
    printf("https://dotrend.dozen.mephi.ru/?dot=%s\n", encoded);

    return ERR_TREE_OK;
}
