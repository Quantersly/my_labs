#ifndef LOG_GAME_H
#define LOG_GAME_H

#include <stdio.h>
#include <stdlib.h>

#ifdef USE_VECTOR

typedef struct Stack {
    int** data;
    int size;
    int first;
    int last;
} Stack;

#else 

typedef struct Node {
    int* game;
    struct Node* prev;
} Node;

typedef struct Stack {
    Node* top;
    int size;
} Stack;

#endif

int push_node(Stack*, int*);
void pop_node(Stack*, int**, int*);
void erase_stack(Stack*);

#endif
