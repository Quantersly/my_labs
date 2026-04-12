#include <stdio.h>
#include <stdlib.h>

#include <time.h>

#include "my_string.h"

#define DELIM " \t"
#define PROMT "[. ݁₊ ⊹ . ݁ ⟡ ݁ . ⊹ ₊ ݁.]"

char *process(const char *);

int main() {
    char *input = my_readline(PROMT);
    while (input != NULL) {
        printf("\"%s\"\n", input);
        clock_t start = clock();
        char *output = process(input);
        if (output == NULL) {
            printf("Аварийное завершение программы!");
            free(input);
            free(output);
            return 0;
        }
        clock_t end = clock();
        float t = (float)(end - start) / CLOCKS_PER_SEC;
        printf("\"%s\"\n", output);
        printf("time - %f\n", t);
        free(input);
        free(output);
        input = my_readline(PROMT);
    }
    return 0;
}

char *process(const char *str) {
    char *s = my_strdup(str);
    int s_len = my_strlen(s);
    char *res = (char*)calloc(s_len + 1, sizeof(char));
    if (res == NULL) return NULL;
    int len = 0;
    char *word = my_strtok(s, DELIM);
    while (word != NULL) {
        int w_len = my_strlen(word);
        if (w_len % 2 == 0) {
            my_memcpy(res + len, word, w_len * sizeof(char));
            len += w_len;
            res[len] = ' ';
            ++len;
        }
        word = my_strtok(NULL, DELIM);
    }
    free(s);
    if (len > 0) {
        --len;
    }
    res[len] = '\0';
    res = (char*)realloc(res, (len + 1) * sizeof(char));
    return res;
}

