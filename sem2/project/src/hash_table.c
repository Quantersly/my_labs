#include "hash_table.h"
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define HT_INITIAL_CAP 64
#define HT_LOAD_THRESH 0.85

typedef enum {
    EMPTY,
    OCCUPIED,
    DELETED
} SlotState;

typedef struct {
    SlotState state;
    char* key;
} Slot;

typedef struct {
    Slot* table;
    size_t capacity;
    size_t size;
} HashTable;

static size_t _hash(const char* key, size_t cap) {
    // В основе хеш-функция Дэниела Бернштейна :]
    // А число 5381 и правда магическое]]
    size_t h = 5381;
    int c;
    while ((c = (unsigned char)*key++) != '\0')
        h = ((h << 5) + h) + c;
    return h % cap;
}

static size_t _probe(size_t h0, size_t i, size_t cap) {
    // А тут магическая формула, которую доказал Радке
    size_t idx = (h0 + (i * i + i) / 2) % cap;
    return idx;
}

void* hash_table_create(void) {
    HashTable* ht = calloc(1, sizeof(HashTable));
    if (!ht) return NULL;
    ht->capacity = HT_INITIAL_CAP;
    ht->table = calloc(ht->capacity, sizeof(Slot));
    if (!ht->table) {
        free(ht);
        return NULL;
    }
    return (void*)ht;
}

static Slot* _find_slot(HashTable* ht, const char* key, size_t* out_i) {
    size_t h0 = _hash(key, ht->capacity);
    for (size_t i = 0; i < ht->capacity; i++) {
        size_t idx = _probe(h0, i, ht->capacity);
        Slot* s = &ht->table[idx];
        if (s->state == EMPTY) {
            if (out_i) *out_i = idx;
            return s;
        }
        if (s->state == OCCUPIED && strcmp(s->key, key) == 0) {
            if (out_i) *out_i = idx;
            return s;
        }
    }
    return NULL;
}

static int _resize(HashTable* ht) {
    size_t new_cap = ht->capacity * 2;
    Slot* new_table = calloc(new_cap, sizeof(Slot));
    if (!new_table) return -1;

    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->table[i].state == OCCUPIED) {
            char* k = ht->table[i].key;
            size_t h0 = _hash(k, new_cap);
            for (size_t j = 0; j < new_cap; j++) {
                size_t idx = _probe(h0, j, new_cap);
                if (new_table[idx].state == EMPTY) {
                    new_table[idx].state = OCCUPIED;
                    new_table[idx].key = k;
                    break;
                }
            }
        }
    }
    free(ht->table);
    ht->table = new_table;
    ht->capacity = new_cap;
    return 0;
}

int hash_table_insert(void* ht_, const char* key) {
    HashTable* ht = ht_;
    if (!ht || !key) return -1;

    if ((double)ht->size / ht->capacity > HT_LOAD_THRESH) {
        if (_resize(ht) != 0) return -1;
    }

    Slot* s = _find_slot(ht, key, NULL);
    if (!s) return -1;

    if (s->state == OCCUPIED) return 0;

    s->state = OCCUPIED;
    s->key = strdup(key);
    if (!s->key) { s->state = EMPTY; return -1; }
    ht->size++;
    return 0;
}

int hash_table_find(void* ht_, const char* key) {
    HashTable* ht = ht_;
    if (!ht || !key) return 0;
    Slot* s = _find_slot(ht, key, NULL);
    return (s && s->state == OCCUPIED) ? 1 : 0;
}

int hash_table_remove(void* ht_, const char* key) {
    HashTable* ht = ht_;
    if (!ht || !key) return -1;
    size_t idx;
    Slot* s = _find_slot(ht, key, &idx);
    if (!s || s->state != OCCUPIED) return -1;
    free(s->key);
    s->key = NULL;
    s->state = DELETED;
    ht->size--;
    return 0;
}

size_t hash_table_size(const void* ht_) {
    const HashTable* ht = ht_;
    return ht ? ht->size : 0;
}

void hash_table_destroy(void* ht_) {
    if (!ht_) return;
    HashTable* ht = ht_;
    for (size_t i = 0; i < ht->capacity; i++) {
        if (ht->table[i].state == OCCUPIED)
            free(ht->table[i].key);
    }
    free(ht->table);
    free(ht);
}
