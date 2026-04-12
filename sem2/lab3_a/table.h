#ifndef TABLE_H
#define TABLE_H

typedef unsigned int KeyType;
typedef unsigned int InfoType;
typedef size_t IndexType;

typedef enum {
    ERR_TABLE_EOF = -1,
    ERR_TABLE_OK = 0,
    ERR_TABLE_VOID_NAME,
    ERR_TABLE_DUBLICATE_KEY,
    ERR_TABLE_NON_POSITIVE_VAR,
    ERR_TABLE_PARENT_NOT_FOUND,
    ERR_TABLE_FULL,
    ERR_TABLE_FULL_IMPORT,
    ERR_TABLE_NOT_FOUND,
    ERR_TABLE_FILE_NOT_FOUND,
    ERR_TABLE_FILE_INCORRECT,
    ERR_TABLE_MEM
} Err_table;

typedef struct {
    KeyType key;
    KeyType par;
    InfoType *info;
} KeySpace;


typedef struct {
    KeySpace *ks;
    IndexType msize;
    IndexType csize;
} Table;


Err_table table_create(Table**, IndexType);
Err_table table_insert(Table*, KeyType, KeyType, InfoType);
Err_table table_import(Table**, const char*, IndexType);
void table_erase(Table**);
Err_table table_search(const Table*, Table**, KeyType);
Err_table table_search_parent(const Table*, Table**, KeyType);
void table_print(const Table*);
Err_table table_delete(Table*, KeyType);

#endif
