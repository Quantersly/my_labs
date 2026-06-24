#ifndef WORDLE_UI_H
#define WORDLE_UI_H

#include "wordle.h"
#include <stdbool.h>

int wordle_ui_run(void);
void wordle_ui_draw_cell(int row, int col, char letter, LetterState state);
void wordle_ui_draw_keyboard(const WordleGame* g);
void wordle_ui_message(const char* msg);
void wordle_ui_draw_board(const WordleGame* g);

#endif
