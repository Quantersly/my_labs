#ifndef MY_STRING_H
#define MY_STRING_H

#include <stdio.h>
#include <stdlib.h>

typedef enum Err_yn{
    ERR_YES = 1,
    ERR_NO = 0
} Err_yn;

char *my_readline(char const * const);
size_t my_strlen(const char *);
void *my_memcpy(void*, const void*, size_t);
char *my_strdup(const char *);
char *my_strtok(char*, const char *);
char *my_strchr(const char *, int);
char *my_strpbrk(const char *, const char *);
size_t my_strspn(const char *, const char *);

#endif
