#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dial.h"
#include "input.h"

void dial_exit(void) {
    printf("До свидания!\n");
}

Err_tree dial_create(Tree **tree) {
    if (*tree != NULL) {
        tree_free(*tree);
        *tree = NULL;
        printf("Старое дерево удалено.\n");
    }
    Err_tree res = tree_create(tree);
    if (res == ERR_TREE_OK)
        printf("Дерево успешно создано.\n");
    return res;
}

Err_tree dial_insert(Tree *tree) {
    printf("Введите ключ: ");
    char *key = NULL;
    Err_inp res = input_string(&key);
    if (res == ERR_INPUT_EOF) {
        free(key);
        return ERR_TREE_EOF;
    }

    printf("Введите значение: ");
    int value = 0;
    res = input_non_negative_int(&value);
    if (res == ERR_INPUT_EOF) {
        free(key);
        return ERR_TREE_EOF;
    }

    InfoType old_value = 0;
    Err_tree result = tree_insert(tree, key, (InfoType)value, &old_value);
    free(key);

    if (result == ERR_TREE_KEY_UPDATED)
        printf(YEL "Ключ уже существовал, старое значение: %u\n" RESET, old_value);

    return result;
}

Err_tree dial_delete(Tree *tree) {
    printf("Введите ключ для удаления: ");
    char *key = NULL;
    Err_inp res = input_string(&key);
    if (res == ERR_INPUT_EOF) {
        free(key);
        return ERR_TREE_EOF;
    }

    Err_tree result = tree_delete(tree, key);
    free(key);
    return result;
}

Err_tree dial_traverse(Tree *tree) {
    printf("Введите ключ-ограничение (или пустую строку для всего дерева): ");
    char *limit = NULL;
    Err_inp res = input_string(&limit);
    if (res == ERR_INPUT_EOF) {
        free(limit);
        return ERR_TREE_EOF;
    }

    const char *limit_ptr = (strlen(limit) == 0) ? NULL : limit;

    Node **out = NULL;
    size_t out_size = 0;
    Err_tree err = tree_traverse(tree, limit_ptr, &out, &out_size);

    if (err == ERR_TREE_OK) {
        printf(GRN "\nОбход в обратном порядке:\n" RESET);
        for (size_t i = 0; i < out_size; i++)
            printf("  %s -> %u\n", out[i]->key, *(out[i]->info));
        free(out);
    }

    free(limit);
    return err;
}

Err_tree dial_search(Tree *tree) {
    printf("Введите ключ для поиска: ");
    char *key = NULL;
    Err_inp res = input_string(&key);
    if (res == ERR_INPUT_EOF) {
        free(key);
        return ERR_TREE_EOF;
    }

    Node **out = NULL;
    size_t out_size = 0;
    Err_tree err = tree_search(tree, key, &out, &out_size);
    free(key);

    if (err == ERR_TREE_OK) {
        printf(GRN "Найдено %zu элемент(ов):\n" RESET, out_size);
        for (size_t i = 0; i < out_size; i++)
            printf("  %s -> %u\n", out[i]->key, *(out[i]->info));
        free(out);
    }

    return err;
}

Err_tree dial_search_closest(Tree *tree) {
    printf("Введите ключ для поиска ближайшего: ");
    char *key = NULL;
    Err_inp res = input_string(&key);
    if (res == ERR_INPUT_EOF) {
        free(key);
        return ERR_TREE_EOF;
    }

    Node **out = NULL;
    size_t out_size = 0;
    Err_tree err = tree_search_closest(tree, key, &out, &out_size);
    free(key);

    if (err == ERR_TREE_OK) {
        printf(GRN "Найдено ближайших %zu элемент(ов):\n" RESET, out_size);
        for (size_t i = 0; i < out_size; i++)
            printf("  %s -> %u\n", out[i]->key, *(out[i]->info));
        free(out);
    }

    return err;
}

Err_tree dial_import(Tree *tree) {
    printf("Введите имя файла: ");
    char *filename = NULL;
    Err_inp res = input_string(&filename);
    if (res == ERR_INPUT_EOF) {
        free(filename);
        return ERR_TREE_EOF;
    }

    if (strlen(filename) == 0) {
        free(filename);
        return ERR_TREE_FILE_NOT_FOUND;
    }

    Err_tree err = tree_import(tree, filename);
    free(filename);
    return err;
}

Err_tree dial_graphviz(Tree *tree) {
    return tree_graphviz(tree);
}