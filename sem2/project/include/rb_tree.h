#ifndef RB_TREE_H
#define RB_TREE_H

#include "dict.h"

void* rb_tree_create(void);
int rb_tree_insert(void*, const char*);
int rb_tree_find(void*, const char*);
int rb_tree_remove(void*, const char*);
size_t rb_tree_size(const void*);
void rb_tree_destroy(void*);

#endif
