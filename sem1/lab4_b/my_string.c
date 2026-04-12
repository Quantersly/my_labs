#include <stdio.h>
#include <stdlib.h>

#include "my_string.h"


size_t my_strlen(const char * str) {
    size_t len = 0;
    while (str[len]) {
        ++len;
    }
    return len;
}


void *my_memcpy(void *ptr_to, const void* ptr_from, size_t n) {
    unsigned char *ptr_t = (unsigned char *)ptr_to;
    const unsigned char *ptr_f = (const unsigned char *)ptr_from;
    if (!ptr_t || !ptr_f) return ptr_to;
    while (n != 0) {
        *ptr_t++ = *ptr_f++;
        n--;
    }
    return ptr_to;
}


char *my_strdup(const char * str) {
    if (str == NULL) {
        return NULL;
    }
    int len = my_strlen(str);
    char *s = "";
    s = (char*)calloc(len + 1, sizeof(char));
    my_memcpy(s, str, len + 1);
    return s;
}


char *my_strtok(char *str, const char *delim) {
    static char *last_pos = NULL;
    if (str != NULL) {
        last_pos = str;
    } else if (last_pos == NULL) {
        return NULL;
    }
    size_t count = my_strspn(last_pos, delim);
    last_pos += count;
    if (!(*last_pos)) {
        last_pos = NULL;
        return NULL;
    }
    char *start_pos = last_pos;
    last_pos = my_strpbrk(last_pos, delim);
    if (last_pos) {
        *last_pos = '\0';
        last_pos++;
    }
    return start_pos;
}


char *my_strchr(const char *delim, int symbol) {
    if (!delim) return NULL;
    size_t el = 0;
    while (delim[el]) {
        if (delim[el] == symbol) {
            return (char*)(delim + el);
        }
        el++;
    }
    return NULL;
}


char *my_strpbrk(const char *str, const char *delim) {
    size_t el = 0;
    while (str[el]) {
        char *pos = my_strchr(delim, str[el]);
        if (pos) {
            return (char*)(str + el);
        }
        el++;
    }
    return NULL;
}


size_t my_strspn(const char *str, const char *delim) {
    if (!str || !delim) return 0; 
    size_t el = 0;
    while (str[el]) {
        char *pos = my_strchr(delim, str[el]);
        if (!pos) {
            return el;
        }
        el++;
    }
    return el;
}


char *my_readline(char const * const promt) {
    char buf[81] = {0};
    char *res = NULL;
    int len = 0;
    int n = 0;
    printf("%s", promt);
    do {
        n = scanf("%80[^\n]", buf);
        if (n < 0) {
            if (!res) {
                return NULL;
            }
        } else if (n > 0) {
            int chunk_len = my_strlen(buf);
            int str_len = len + chunk_len;
            res = realloc(res, str_len + 1);
            my_memcpy(res + len, buf, chunk_len);
            len = str_len;
        } else {
            scanf("%*c");
        }
    } while (n > 0);
    if (len > 0) {
        res[len] = '\0';
    } else {
        res = calloc(1, sizeof(char));
    }
    return res;
}

