#include "log_game.h"
#include <string.h>

#define COUNT_CELL 16
#define CORRECT_CELL 1

int push_node(Stack *stack, int* array) {
    Node* new_node = (Node*)calloc(1, sizeof(Node));
    if (new_node == NULL) {
        return -1;
    }
    new_node->game = (int*)calloc(COUNT_CELL + CORRECT_CELL, sizeof(int));
    if (new_node->game == NULL) {
        free(new_node);
        return -1;
    }

    memcpy(new_node->game, array, (COUNT_CELL + CORRECT_CELL) * sizeof(int));
    if (stack->top == NULL) {
        new_node->prev = NULL;
    } else {
        new_node->prev = stack->top;
    }
    stack->top = new_node;
    (stack->size)++;
    return 0;
}

void pop_node(Stack *stack, int **array, int *void_cell) {
    if (stack->size > 1) {
        Node* old_node = stack->top;
        stack->top = stack->top->prev;
        free(old_node->game);
        free(old_node);
        memcpy(*array, stack->top->game, (COUNT_CELL + CORRECT_CELL) * sizeof(int));
        (stack->size)--;
        for (int i = 0; i < COUNT_CELL; i++) {
            if ((*array)[i] == 0) {
                *void_cell = i;
            }
        }
    }
}

void erase_stack(Stack *stack) {
    do {
        Node* old_node = stack->top;
        stack->top = stack->top->prev;
        free(old_node->game);
        free(old_node); 
    } while (stack->top != NULL);
    free(stack);
}
