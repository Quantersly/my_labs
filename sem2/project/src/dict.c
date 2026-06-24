#include "dict.h"
#include "hash_table.h"
#include "rb_tree.h"
#include <stdlib.h>

struct Dict {
    void* impl;
    int (*insert)(void*, const char*);
    int (*find)(void*, const char*);
    int (*remove)(void*, const char*);
    size_t (*size)(const void*);
    void (*destroy)(void*);
};

Dict* dict_create(void) {
    Dict* d = calloc(1, sizeof(Dict));
    if (!d) return NULL;

#if USE_RB_TREE
    d->impl = rb_tree_create();
    d->insert = rb_tree_insert;
    d->find = rb_tree_find;
    d->remove = rb_tree_remove;
    d->size = rb_tree_size;
    d->destroy = rb_tree_destroy;
#else
    d->impl = hash_table_create();
    d->insert = hash_table_insert;
    d->find = hash_table_find;
    d->remove = hash_table_remove;
    d->size = hash_table_size;
    d->destroy = hash_table_destroy;
#endif

    if (!d->impl) {
        free(d);
        return NULL;
    }
    return d;
}

void dict_destroy(Dict* d) {
    if (!d) return;
    d->destroy(d->impl);
    free(d);
}

int dict_insert(Dict* d, const char* key) {
    return d ? d->insert(d->impl, key) : -1;
}

int dict_find(Dict* d, const char* key) {
    return d ? d->find(d->impl, key) : 0;
}

int dict_remove(Dict* d, const char* key) {
    return d ? d->remove(d->impl, key) : -1;
}

size_t dict_size(const Dict* d) {
    return d ? d->size(d->impl) : 0;
}
