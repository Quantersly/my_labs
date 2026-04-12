#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

#include "game.h"
#include "log_game.h"

#define COUNT_CELL 16
#define CORRECT_CELL 1
#define VOID_CELL 1


Err_tag start_game(int **array, Stack **stack) {
    *array = (int*)calloc(COUNT_CELL + CORRECT_CELL, sizeof(int));
    if (*array == NULL) {
        free(*array);
        return ERR_TAG_MEM;
    }
    *stack = (Stack*)calloc(1, sizeof(Stack));
    if (*stack == NULL) {
        free(*stack);
        return ERR_TAG_MEM;
    }
    for (int i = 0; i < COUNT_CELL - VOID_CELL; i++) (*array)[i] = i + 1;
    for (int i = COUNT_CELL - VOID_CELL - 1; i > -1; i--) {
        int j = rand() % (i + 1);
        int temp = (*array)[i];
        (*array)[i] = (*array)[j];
        (*array)[j] = temp;
    }
    int n = 0;
    for (int i = 0; i < CORRECT_CELL - VOID_CELL; i++) {
        for (int j = i - 1; j > -1; j--) {
            if ((*array)[j] > (*array)[i]) {
                n++;
            }
        }
    }
    if (n % 2 != 0) {
        int temp_2 = (*array)[0];
        (*array)[0] = (*array)[1];
        (*array)[1] = temp_2;
    }
    for (int i = 0; i < COUNT_CELL - VOID_CELL; i++) {
        if ((*array)[i] == i + 1) {
            ((*array)[COUNT_CELL + CORRECT_CELL - 1])++;
        }
    }
    int res = push_node(*stack, *array);
    if (res != 0) {
        return ERR_TAG_MEM;
    }
    return ERR_TAG_OK;
}


Err_tag test_game(int **array, Stack **stack) {
    *array = (int*)calloc(COUNT_CELL + CORRECT_CELL, sizeof(int));
    if (*array == NULL) {
        free(*array);
        return ERR_TAG_MEM;
    }
    *stack = (Stack*)calloc(1, sizeof(Stack));
    if (*stack == NULL) {
        free(*stack);
        return ERR_TAG_MEM;
    }
    for (int i = 0; i < COUNT_CELL - VOID_CELL; i++) (*array)[i] = i + 1;
    (*array)[10] = 0;
    (*array)[11] = 11;
    (*array)[15] = 12;
    (*array)[16] = 13;
    int res = push_node(*stack, *array);
    if (res != 0) {
        return ERR_TAG_MEM;
    }
    return ERR_TAG_OK;
}


Err_tag swap_cell(int *array, int j, int i, Stack *stack) {
    if (array[i] == i + 1) {
        (array[COUNT_CELL + CORRECT_CELL - 1])--;
    } 
    if (array[j] == j + 1) {
        (array[COUNT_CELL + CORRECT_CELL - 1])--;
    }
    int temp = array[j];
    array[j] = array[i];
    array[i] = temp;
    if (array[i] == i + 1) {
        (array[COUNT_CELL + CORRECT_CELL - 1])++;
    }
    if (array[j] == j + 1) {
        (array[COUNT_CELL + CORRECT_CELL - 1])++;
    }
    int res = push_node(stack, array);
    if (res != 0) {
        return ERR_TAG_MEM;
    }
    return ERR_TAG_OK;
}


void print_tag(int *array) {
    clear();
    for (int i = 0; i < 4; i++) {
        printw("#####################\n");
        printw("#    #    #    #    #\n");
        for (int j = 0; j < 4; j++) {
            printw("# ");
            if (array[i * 4 + j] != 0) {
                printw("%02d ", array[i * 4 + j]);
            } else {
                printw("   ");
            }
        }
        printw("#\n");
        printw("#    #    #    #    #\n");
    }
    printw("#####################\n");
}
