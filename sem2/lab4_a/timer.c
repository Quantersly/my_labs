#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "tree.h"

#define MAX_SIZE 100000
#define STEP     5000
#define REPEATS  200
#define RUNS     5

static void generate_key(char *buf, int i) {
    sprintf(buf, "key%d", i);
}

static double measure_ns(struct timespec t1, struct timespec t2, int repeats) {
    return ((t2.tv_sec - t1.tv_sec) * 1e9 + (t2.tv_nsec - t1.tv_nsec)) / repeats;
}

static void shuffle(int *arr, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        int tmp = arr[i];
        arr[i] = arr[j];
        arr[j] = tmp;
    }
}

int main(void) {
    srand(42);
    printf("size,insert,search,traverse,delete\n");

    char key[64];

    for (int n = STEP; n <= MAX_SIZE; n += STEP) {
        double sum_insert = 0, sum_search = 0, sum_traverse = 0, sum_delete = 0;

        int *indices = malloc(n * sizeof(int));
        if (!indices) {
            fprintf(stderr, "malloc failed\n");
            return 1;
        }
        for (int i = 0; i < n; i++)
            indices[i] = i;

        for (int run = 0; run < RUNS; run++) {
            Tree *tree = NULL;
            tree_create(&tree);

            shuffle(indices, n);
            for (int i = 0; i < n; i++) {
                generate_key(key, indices[i]);
                InfoType old = 0;
                tree_insert(tree, key, (InfoType)i, &old);
            }

            struct timespec t1, t2;

            // insert
            char **test_keys = malloc(REPEATS * sizeof(char *));
            for (int r = 0; r < REPEATS; r++) {
                test_keys[r] = malloc(64);
                generate_key(test_keys[r], n + r * (n / REPEATS));
            }

            clock_gettime(CLOCK_MONOTONIC, &t1);
            for (int r = 0; r < REPEATS; r++) {
                InfoType old = 0;
                tree_insert(tree, test_keys[r], (InfoType)r, &old);
            }
            clock_gettime(CLOCK_MONOTONIC, &t2);
            sum_insert += measure_ns(t1, t2, REPEATS);

            // search
            clock_gettime(CLOCK_MONOTONIC, &t1);
            for (int r = 0; r < REPEATS; r++) {
                Node **out = NULL;
                size_t out_size = 0;
                tree_search(tree, test_keys[r], &out, &out_size);
                free(out);
            }
            clock_gettime(CLOCK_MONOTONIC, &t2);
            sum_search += measure_ns(t1, t2, REPEATS);

            // traverse
            clock_gettime(CLOCK_MONOTONIC, &t1);
            for (int r = 0; r < REPEATS; r++) {
                Node **out = NULL;
                size_t out_size = 0;
                tree_traverse(tree, NULL, &out, &out_size);
                free(out);
            }
            clock_gettime(CLOCK_MONOTONIC, &t2);
            sum_traverse += measure_ns(t1, t2, REPEATS);

            // delete
            clock_gettime(CLOCK_MONOTONIC, &t1);
            for (int r = 0; r < REPEATS; r++) {
                tree_delete(tree, test_keys[r]);
            }
            clock_gettime(CLOCK_MONOTONIC, &t2);
            sum_delete += measure_ns(t1, t2, REPEATS);

            for (int r = 0; r < REPEATS; r++) {
                free(test_keys[r]);
            }
            free(test_keys);

            tree_free(tree);
        }

        free(indices);

        printf("%d,%.1f,%.1f,%.1f,%.1f\n",
               n,
               sum_insert   / RUNS,
               sum_search   / RUNS,
               sum_traverse / RUNS,
               sum_delete   / RUNS);

        fprintf(stderr, "n = %d / %d\n", n, MAX_SIZE);
    }

    return 0;
}