#include "wordle.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

#define MAX_GUESSES 99

struct WordleGame {
    char* target_word;
    int total_attempts;
    int attempts_left;
    int guess_count;
    GuessResult guesses[MAX_GUESSES];
    bool is_over;
    bool is_won;
    Dict* dict;
};

static char _toupper_char(char c) {
    return (char)toupper((unsigned char)c);
}

static void _to_upper_inplace(char* s) {
    if (!s) return;
    for (int i = 0; s[i]; i++)
        s[i] = _toupper_char(s[i]);
}

static char* _strndup(const char* s, size_t n) {
    size_t len = 0;
    while (len < n && s[len]) len++;
    char* d = malloc(len + 1);
    if (!d) return NULL;
    memcpy(d, s, len);
    d[len] = '\0';
    return d;
}

static int _load_dict(Dict* d, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) return -1;

    char line[256];
    int count = 0;
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r')) {
            line[--len] = '\0';
        }
        if (len == 0) continue;
        _to_upper_inplace(line);
        if (dict_insert(d, line) == 0) count++;
    }
    fclose(fp);
    return count > 0 ? count : -1;
}

WordleGame* wordle_create(int attempts, const char* filename) {
    if (attempts < 1 || attempts > WORDLE_MAX_ATTEMPTS) return NULL;
    if (!filename) filename = WORDLE_DICT_FILE;

    WordleGame* g = calloc(1, sizeof(WordleGame));
    if (!g) return NULL;

    g->dict = dict_create();
    if (!g->dict) { free(g); return NULL; }

    if (_load_dict(g->dict, filename) < 0) {
        dict_destroy(g->dict);
        free(g);
        return NULL;
    }

    size_t n_words = dict_size(g->dict);
    if (n_words == 0) {
        dict_destroy(g->dict);
        free(g);
        return NULL;
    }

    FILE* fp = fopen(filename, "r");
    if (!fp) {
        dict_destroy(g->dict);
        free(g);
        return NULL;
    }

    char line[256];
    char** words = NULL;
    size_t count = 0;
    while (fgets(line, sizeof(line), fp)) {
        size_t len = strlen(line);
        while (len > 0 && (line[len-1] == '\n' || line[len-1] == '\r'))
            line[--len] = '\0';
        if (len == 0) continue;
        _to_upper_inplace(line);
        char** tmp = realloc(words, (count + 1) * sizeof(char*));
        if (!tmp) { free(words); fclose(fp); dict_destroy(g->dict); free(g); return NULL; }
        words = tmp;
        words[count] = _strndup(line, WORDLE_WORD_LEN);
        if (!words[count]) { while (count--) free(words[count]); free(words); fclose(fp); dict_destroy(g->dict); free(g); return NULL; }
        count++;
    }
    fclose(fp);
    unsigned int seed = (unsigned int)time(NULL);
    size_t idx = (size_t)(rand_r(&seed) % count);

    if (idx >= count) idx = count - 1;

    g->target_word = words[idx];
    g->total_attempts = attempts;
    g->attempts_left = attempts;
    g->guess_count = 0;
    g->is_over = false;
    g->is_won = false;

    for (size_t i = 0; i < count; i++) {
        if (i != idx) free(words[i]);
    }
    free(words);

    return g;
}

void wordle_destroy(WordleGame* g) {
    if (!g) return;
    if (g->target_word) free(g->target_word);
    if (g->dict) dict_destroy(g->dict);
    free(g);
}

int wordle_try_guess(WordleGame* g, const char* guess) {
    if (!g || !guess) return -2;
    if (g->is_over) return -2;
    if (g->attempts_left <= 0) {
        g->is_over = true;
        return -2;
    }

    char up[WORDLE_WORD_LEN + 1];
    size_t len = 0;
    while (guess[len] && len < WORDLE_WORD_LEN) {
        up[len] = (char)toupper((unsigned char)guess[len]);
        len++;
    }
    up[len] = '\0';
    if (len != WORDLE_WORD_LEN) return -1;

    GuessResult* res = &g->guesses[g->guess_count];
    memcpy(res->letters, up, WORDLE_WORD_LEN);
    res->letters[WORDLE_WORD_LEN] = '\0';
    res->is_valid = (dict_find(g->dict, up) == 1);

    if (!res->is_valid) {
        for (int i = 0; i < WORDLE_WORD_LEN; i++)
            res->states[i] = LS_ABSENT;
        return -1;
    }

    int used[WORDLE_WORD_LEN] = { 0 };

    for (int i = 0; i < WORDLE_WORD_LEN; i++) {
        if (up[i] == g->target_word[i]) {
            res->states[i] = LS_CORRECT;
            used[i] = 1;
        } else {
            res->states[i] = LS_ABSENT;
        }
    }

    for (int i = 0; i < WORDLE_WORD_LEN; i++) {
        if (res->states[i] == LS_CORRECT) continue;
        res->states[i] = LS_ABSENT;
        for (int j = 0; j < WORDLE_WORD_LEN; j++) {
            if (!used[j] && up[i] == g->target_word[j]) {
                res->states[i] = LS_PRESENT;
                used[j] = 1;
                break;
            }
        }
    }

    g->guess_count++;
    g->attempts_left--;

    bool won = true;
    for (int i = 0; i < WORDLE_WORD_LEN; i++) {
        if (res->states[i] != LS_CORRECT) {
            won = false;
            break;
        }
    }

    if (won) {
        g->is_over = true;
        g->is_won  = true;
        return 1;
    }

    if (g->attempts_left <= 0) {
        g->is_over = true;
        g->is_won  = false;
        return -2;
    }

    return 0;
}

int wordle_attempts_left(const WordleGame* g) { return g ? g->attempts_left : 0; }
int wordle_total_attempts(const WordleGame* g) { return g ? g->total_attempts : 0; }
int wordle_guess_count(const WordleGame* g) { return g ? g->guess_count : 0; }
bool wordle_is_over(const WordleGame* g) { return g ? g->is_over : true; }
bool wordle_is_won(const WordleGame* g) { return g ? g->is_won : false; }
const char* wordle_target(const WordleGame* g) { return g ? g->target_word : ""; }

const GuessResult* wordle_get_result(const WordleGame* g, int idx) {
    if (!g || idx < 0 || idx >= g->guess_count) return NULL;
    return &g->guesses[idx];
}
