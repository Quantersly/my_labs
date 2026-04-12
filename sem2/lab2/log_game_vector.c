#include "log_game.h"
#include <string.h>

#define COUNT_CELL 16
#define CORRECT_CELL 1
#define VECTOR_SIZE 10

int push_node(Stack *stack, int* array) {
    if (stack->last == 0) {
        stack->size = VECTOR_SIZE;
        stack->data = (int**)calloc(stack->size, sizeof(int*));
        if (stack->data == NULL) {
              return -1;
        }
    }
    if (stack->last % stack->size == stack->first) {
        free(stack->data[stack->first]);
        stack->first = (stack->first + 1) % stack->size;
    }
    stack->data[stack->last % stack->size] = (int*)calloc(COUNT_CELL + CORRECT_CELL, sizeof(int));
    memcpy(stack->data[stack->last % stack->size], array, (COUNT_CELL + CORRECT_CELL) * sizeof(int));
    (stack->last)++;
    return 0;
}

void pop_node(Stack *stack, int **array, int *void_cell) {
    if ((stack->last - 1) % stack->size != stack->first) {
        (stack->last)--;
        memcpy(*array, stack->data[(stack->last - 1) % stack->size], (COUNT_CELL + CORRECT_CELL) * sizeof(int));
        for (int i = 0; i < COUNT_CELL; i++) {
            if ((*array)[i] == 0) {
                *void_cell = i;
            }
        }
        free(stack->data[stack->last % stack->size]);
        stack->data[stack->last % stack->size] = NULL;
    }
}

void erase_stack(Stack *stack) {
    for (int i = 0; i < stack->size; i++) {
        if (stack->data[i] != NULL) {
            free(stack->data[i]);
        }
    }
    free(stack->data);
    free(stack);
}

