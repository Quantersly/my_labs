#ifndef GAME_H
#define GAME_H

#include "log_game.h"

typedef enum Err_tag {
    ERR_TAG_OK = 0,
    ERR_TAG_MEM = -2
} Err_tag;

Err_tag start_game(int**, Stack**);
Err_tag test_game(int**, Stack**);
Err_tag swap_cell(int*, int, int, Stack*);
void print_tag(int*);

#endif
