#include "rb_tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define RB_RED 0
#define RB_BLACK 1

typedef struct RBNode {
    char* key;
    struct RBNode* left;
    struct RBNode* right;
    struct RBNode* parent;
    int color;
} RBNode;

typedef struct {
    RBNode* root;
    size_t size;
} RBTree;

static RBNode* _new_node(const char* key) {
    RBNode* n = calloc(1, sizeof(RBNode));
    if (!n) return NULL;
    n->key = strdup(key);
    if (!n->key) { free(n); return NULL; }
    n->color = RB_RED;
    return n;
}

static void _rotate_left(RBNode** root, RBNode* x) {
    RBNode* y = x->right;
    x->right = y->left;
    if (y->left)
        y->left->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        *root = y;
    else if (x == x->parent->left)
        x->parent->left = y;
    else
        x->parent->right = y;
    y->left = x;
    x->parent = y;
}

static void _rotate_right(RBNode** root, RBNode* x) {
    RBNode* y = x->left;
    x->left = y->right;
    if (y->right)
        y->right->parent = x;
    y->parent = x->parent;
    if (!x->parent)
        *root = y;
    else if (x == x->parent->right)
        x->parent->right = y;
    else
        x->parent->left = y;
    y->right  = x;
    x->parent = y;
}

static void _insert_fixup(RBNode** root, RBNode* n) {
    while (n->parent && n->parent->color == RB_RED) {
        RBNode* p = n->parent;
        RBNode* g = p->parent;
        if (!g) break;

        if (p == g->left) {
            RBNode* u = g->right;
            if (u && u->color == RB_RED) {
                p->color = RB_BLACK;
                u->color = RB_BLACK;
                g->color = RB_RED;
                n = g;
            } else {
                if (n == p->right) {
                    n = p;
                    _rotate_left(root, n);
                }
                n->parent->color = RB_BLACK;
                n->parent->parent->color = RB_RED;
                _rotate_right(root, n->parent->parent);
            }
        } else {
            RBNode* u = g->left;
            if (u && u->color == RB_RED) {
                p->color = RB_BLACK;
                u->color = RB_BLACK;
                g->color = RB_RED;
                n = g;
            } else {
                if (n == p->left) {
                    n = p;
                    _rotate_right(root, n);
                }
                n->parent->color = RB_BLACK;
                n->parent->parent->color = RB_RED;
                _rotate_left(root, n->parent->parent);
            }
        }
    }
    (*root)->color = RB_BLACK;
}

static RBNode* _search(RBNode* root, const char* key) {
    while (root) {
        int cmp = strcmp(key, root->key);
        if (cmp == 0) return root;
        if (cmp < 0) root = root->left;
        else root = root->right;
    }
    return NULL;
}

static RBNode* _minimum(RBNode* n) {
    while (n->left) n = n->left;
    return n;
}

static void _transplant(RBNode** root, RBNode* u, RBNode* v) {
    if (!u->parent)
        *root = v;
    else if (u == u->parent->left)
        u->parent->left = v;
    else
        u->parent->right = v;
    if (v)
        v->parent = u->parent;
}

static void _erase_fixup(RBNode** root, RBNode* n, RBNode* n_parent) {
    while (n != *root && (!n || n->color == RB_BLACK)) {
        RBNode* p = n ? n->parent : n_parent;
        if (!p) break;

        RBNode* s = (n == p->left) ? p->right : p->left;
        if (!s) { n = p; continue; }

        if (s->color == RB_RED) {
            s->color = RB_BLACK;
            p->color = RB_RED;
            if (n == p->left) {
                _rotate_left(root, p);
                s = p->right;
            } else {
                _rotate_right(root, p);
                s = p->left;
            }
        }
        if (!s) { n = p; continue; }

        int left_black  = !s->left  || s->left->color  == RB_BLACK;
        int right_black = !s->right || s->right->color == RB_BLACK;

        if (left_black && right_black) {
            s->color = RB_RED;
            n = p;
        } else {
            if (n == p->left && right_black) {
                if (s->left)  s->left->color  = RB_BLACK;
                s->color = RB_RED;
                _rotate_right(root, s);
                s = p->right;
            } else if (n == p->right && left_black) {
                if (s->right) s->right->color = RB_BLACK;
                s->color = RB_RED;
                _rotate_left(root, s);
                s = p->left;
            }
            RBNode* pp = p->parent;
            s->color = p->color;
            p->color = RB_BLACK;
            if (n == p->left) {
                if (s->right) s->right->color = RB_BLACK;
                _rotate_left(root, p);
            } else {
                if (s->left)  s->left->color  = RB_BLACK;
                _rotate_right(root, p);
            }
            n = *root;
            (void)pp;
            break;
        }
    }
    if (n) n->color = RB_BLACK;
}

static void _erase(RBNode** root, RBNode* z) {
    RBNode* y = z;
    RBNode* x;
    int y_original_color = y->color;
    if (!z->left) {
        x = z->right;
        _transplant(root, z, z->right);
    } else if (!z->right) {
        x = z->left;
        _transplant(root, z, z->left);
    } else {
        y = _minimum(z->right);
        y_original_color = y->color;
        x = y->right;
        if (y->parent == z) {
            if (x) x->parent = y;
        } else {
            _transplant(root, y, y->right);
            y->right = z->right;
            if (y->right) y->right->parent = y;
        }
        _transplant(root, z, y);
        y->left = z->left;
        if (y->left) y->left->parent = y;
        y->color = z->color;
    }
    free(z->key);
    free(z);
    if (y_original_color == RB_BLACK && x) {
        _erase_fixup(root, x, x->parent);
    }
}

static void _destroy_tree(RBNode* n) {
    if (!n) return;
    _destroy_tree(n->left);
    _destroy_tree(n->right);
    free(n->key);
    free(n);
}

void* rb_tree_create(void) {
    RBTree* t = calloc(1, sizeof(RBTree));
    return t;
}

int rb_tree_insert(void* t_, const char* key) {
    RBTree* t = t_;
    if (!t || !key) return -1;
    RBNode* prev = NULL;
    RBNode* cur  = t->root;
    while (cur) {
        prev = cur;
        int cmp = strcmp(key, cur->key);
        if (cmp == 0) return 0;   /* уже есть */
        cur = (cmp < 0) ? cur->left : cur->right;
    }
    RBNode* n = _new_node(key);
    if (!n) return -1;
    n->parent = prev;
    if (!prev)
        t->root = n;
    else if (strcmp(key, prev->key) < 0)
        prev->left = n;
    else
        prev->right = n;
    t->size++;
    _insert_fixup(&t->root, n);
    return 0;
}

int rb_tree_find(void* t_, const char* key) {
    RBTree* t = t_;
    if (!t || !key) return 0;
    return _search(t->root, key) != NULL ? 1 : 0;
}

int rb_tree_remove(void* t_, const char* key) {
    RBTree* t = t_;
    if (!t || !key) return -1;
    RBNode* n = _search(t->root, key);
    if (!n) return -1;
    _erase(&t->root, n);
    t->size--;
    return 0;
}

size_t rb_tree_size(const void* t_) {
    const RBTree* t = t_;
    return t ? t->size : 0;
}

void rb_tree_destroy(void* t_) {
    if (!t_) return;
    RBTree* t = t_;
    _destroy_tree(t->root);
    free(t);
}
