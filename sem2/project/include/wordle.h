#ifndef WORDLE_H
#define WORDLE_H

#include "dict.h"
#include <stdbool.h>
#include <time.h>

#define WORDLE_WORD_LEN 5
#define WORDLE_MAX_ATTEMPTS 99

typedef enum {
    LS_ABSENT = 0,
    LS_CORRECT = 1,
    LS_PRESENT = 2 
} LetterState;

typedef struct {
    char letters[WORDLE_WORD_LEN + 1];
    LetterState states[WORDLE_WORD_LEN];
    bool is_valid;
} GuessResult;

typedef struct WordleGame WordleGame;

WordleGame* wordle_create(int attempts, const char* filename);
void wordle_destroy(WordleGame* g);
int wordle_try_guess(WordleGame* g, const char* guess);
int wordle_attempts_left(const WordleGame* g);
int wordle_total_attempts(const WordleGame* g);
int wordle_guess_count(const WordleGame* g);
bool wordle_is_over(const WordleGame* g);
bool wordle_is_won(const WordleGame* g);
const GuessResult* wordle_get_result(const WordleGame* g, int guess_index);
const char* wordle_target(const WordleGame* g);

#define WORDLE_DICT_FILE "words.txt"

#endif
