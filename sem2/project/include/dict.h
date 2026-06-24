#ifndef DICT_H
#define DICT_H

#include <stddef.h>

typedef struct Dict Dict;

Dict* dict_create(void);
void dict_destroy(Dict* d);
int dict_insert(Dict* d, const char* key);
int dict_find(Dict* d, const char* key);
int dict_remove(Dict* d, const char* key);
size_t dict_size(const Dict* d);

#endif
