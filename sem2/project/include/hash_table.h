#ifndef HASH_TABLE_H
#define HASH_TABLE_H

#include "dict.h"

void* hash_table_create(void);
int hash_table_insert(void*, const char*);
int hash_table_find(void*, const char*);
int hash_table_remove(void*, const char*);
size_t hash_table_size(const void*);
void hash_table_destroy(void*);

#endif
