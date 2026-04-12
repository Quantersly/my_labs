#include <stdio.h>
#include <stdlib.h>

#include "table.h"


Err_table table_insert(Table *my_table, KeyType key, KeyType par, InfoType info) {
    if (!my_table) {
        return ERR_TABLE_FULL;
    }
    if (key == 0) {
        return ERR_TABLE_NON_POSITIVE_VAR;
    }
    IndexType csize = my_table->csize;
    if (csize >= my_table->msize) {
        return ERR_TABLE_FULL;
    }
    int have_parent = 0;
    for (IndexType i = 0; i < csize; i++) {
        if (my_table->ks[i].key == key) {
            return ERR_TABLE_DUBLICATE_KEY;
        }
        if (my_table->ks[i].key == par) {
            have_parent = 1;
        }
    }
    if (have_parent == 0 && par != 0) {
        return ERR_TABLE_PARENT_NOT_FOUND;
    }
    my_table->ks[csize].info = (InfoType*)calloc(1, sizeof(InfoType));
    if (my_table->ks[csize].info == NULL) {
        return ERR_TABLE_MEM;
    }
    *(my_table->ks[csize].info) = info;
    my_table->ks[csize].key = key;
    my_table->ks[csize].par = par;
    (my_table->csize)++;
    return ERR_TABLE_OK;
}

Err_table table_create(Table **my_table, IndexType msize) {
    table_erase(my_table);
    (*my_table) = (Table*)calloc(1, sizeof(Table));
    if (*my_table == NULL) {
        return ERR_TABLE_MEM;
    }
    (*my_table)->ks = (KeySpace*)calloc(msize, sizeof(KeySpace));
    if ((*my_table)->ks == NULL) {
        free(*my_table);
        *my_table = NULL;
        return ERR_TABLE_MEM;
    }
    (*my_table)->msize = msize;
    return ERR_TABLE_OK; 
}

Err_table table_import(Table **my_table, const char *name_file, IndexType msize) {
    FILE *f = fopen(name_file, "r");
    if (!f) return ERR_TABLE_FILE_NOT_FOUND;
    
    Err_table res_cre = table_create(my_table, msize);
    if (res_cre != ERR_TABLE_OK) {
        table_erase(my_table);
        return res_cre;
    }
    int key, par, info, res;
    while ((res = fscanf(f, "%d %d %d", &key, &par, &info)) == 3) {
        Err_table res_ins = ERR_TABLE_OK;
        if ((*my_table)->csize == (*my_table)->msize) {
            table_erase(my_table);
            fclose(f);
            return ERR_TABLE_FULL_IMPORT;
        }
        if (key <= 0 || par < 0 || info < 0) {
            fclose(f);
            table_erase(my_table);
            return ERR_TABLE_NON_POSITIVE_VAR;
        }
        res_ins = table_insert(*my_table, (KeyType)key, (KeyType)par, (InfoType)info);
        if (res_ins != ERR_TABLE_OK) {
            fclose(f);
            table_erase(my_table);
            return res_ins;
        }
    }
    if (res != EOF) {
        fclose(f);
        table_erase(my_table);
        return ERR_TABLE_FILE_INCORRECT;
    }
    fclose(f);
    return ERR_TABLE_OK;
}

void table_erase(Table **tab) {
    Table *my_table = (*tab);
    if (my_table != NULL) {
        if (my_table->ks != NULL) {
            for (IndexType i = 0; i < my_table->csize; i++) {
                if (my_table->ks[i].info != NULL) {
                    free(my_table->ks[i].info);
                    my_table->ks[i].info = NULL;
                }
            }
            free(my_table->ks);
            my_table->ks = NULL;
        }
        free(my_table);
    }
    *tab = NULL;
}

Err_table table_search(const Table *my_table, Table **ans_table, KeyType key) {
    if (!my_table) {
        return ERR_TABLE_NOT_FOUND;
    }
    Err_table res_ins = table_create(ans_table, my_table->msize);
    if (res_ins != ERR_TABLE_OK) {
        return res_ins;
    }
    for (IndexType i = 0; i < my_table->csize; i++) {
        if (my_table->ks[i].key == key) {
            (*ans_table)->ks[(*ans_table)->csize].info = (InfoType*)calloc(1, sizeof(InfoType));
            if ((*ans_table)->ks[(*ans_table)->csize].info == NULL) {
                table_erase(ans_table);
                return ERR_TABLE_MEM;
            }
            (*ans_table)->ks[(*ans_table)->csize].key = key;
            (*ans_table)->ks[(*ans_table)->csize].par = my_table->ks[i].par;
            (*((*ans_table)->ks[(*ans_table)->csize].info)) = *(my_table->ks[i].info);
            ((*ans_table)->csize)++;
        }
    }
    if ((*ans_table)->csize == 0) {
        return ERR_TABLE_NOT_FOUND;
    }
    return ERR_TABLE_OK;
}

Err_table table_search_parent(const Table *my_table, Table **ans_table, KeyType par) {
    if (!my_table) {
        return ERR_TABLE_NOT_FOUND;
    }
    Err_table res_ins = table_create(ans_table, my_table->msize);
    if (res_ins != ERR_TABLE_OK) {
        return res_ins;
    }
 
    (*ans_table)->msize = my_table->msize;
    for (IndexType i = 0; i < my_table->csize; i++) {
        if (my_table->ks[i].par == par) {
            (*ans_table)->ks[(*ans_table)->csize].info = (InfoType*)calloc(1, sizeof(InfoType));
            if ((*ans_table)->ks[(*ans_table)->csize].info == NULL) {
                table_erase(ans_table);
                return ERR_TABLE_MEM;
            }
            (*ans_table)->ks[(*ans_table)->csize].key = my_table->ks[i].key;
            (*ans_table)->ks[(*ans_table)->csize].par = par;
            (*((*ans_table)->ks[(*ans_table)->csize].info)) = *(my_table->ks[i].info);
            ((*ans_table)->csize)++;
        }
    }
    if ((*ans_table)->csize == 0) {
        return ERR_TABLE_NOT_FOUND;
    }
    return ERR_TABLE_OK;
}

void table_print(const Table *my_table) {
    if (!my_table) {
        printf("Таблица не существует (NULL)\n");
        return;
    }
    if (my_table->csize == 0) {
        printf("Таблица пустая\n");
    }
    printf("--- Содержимое таблицы (csize: %lu) ---\n", my_table->csize);
    for (IndexType i = 0; i < my_table->csize; i++) {
        printf("ks[%3lu] | Key: %5u | Parent: %5u | Info: ", i, my_table->ks[i].key, my_table->ks[i].par);
        if (my_table->ks[i].info) {
            printf("%10u\n", *(my_table->ks[i].info));
        } else {
            printf("NULL\n");
        }
    }
    printf("--------------------------------------\n");
}

Err_table table_delete(Table *my_table, KeyType key) {
    if (!my_table) {
        return ERR_TABLE_NOT_FOUND;
    }
    for (IndexType i = 0; i < my_table->csize; i++) {
        if (my_table->ks[i].par == key) {
            my_table->ks[i].par = 0;
        }
        if (my_table->ks[i].key == key) {
            free(my_table->ks[i].info);
            for (IndexType j = i + 1; j < my_table->csize; j++) {
                if (my_table->ks[j].par == key) {
                    my_table->ks[j].par = 0;
                }
                my_table->ks[j - 1] = my_table->ks[j];
            }
            (my_table->csize)--;
            return ERR_TABLE_OK;
        }
    }
    return ERR_TABLE_NOT_FOUND;
}

