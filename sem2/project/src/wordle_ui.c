#include "wordle_ui.h"
#include "wordle.h"
#include <ncurses.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define CELL_W  6
#define CELL_H  3

enum { 
    C_DEFAULT = 0,
    C_GREEN,
    C_YELLOW,
    C_BLUE
};
static bool g_has_color = false;

static void _init_colors(void) {
    if (!has_colors()) return;
    if (start_color() != OK) return;
    use_default_colors();
    init_pair(C_GREEN,  COLOR_BLACK, COLOR_GREEN);
    init_pair(C_YELLOW, COLOR_BLACK, COLOR_YELLOW);
    init_pair(C_BLUE,   COLOR_BLACK, COLOR_CYAN);
    init_pair(C_DEFAULT, COLOR_BLACK, COLOR_WHITE);
    g_has_color = true;
}

static void _draw_cell(int y, int x, char letter, LetterState state) {
    int pair = (state == LS_CORRECT) ? C_GREEN : (state == LS_PRESENT) ? C_YELLOW : C_BLUE;

    if (g_has_color) attron(COLOR_PAIR(pair) | A_BOLD);
    for (int dy = 0; dy < CELL_H; dy++)
        for (int dx = 0; dx < CELL_W; dx++)
            mvaddch(y + dy, x + dx, ' ');
    attrset(A_NORMAL);

    for (int dx = 1; dx < CELL_W - 1; dx++) {
        mvaddch(y, x + dx, '-');
        mvaddch(y + CELL_H - 1, x + dx, '-');
    }
    for (int dy = 1; dy < CELL_H - 1; dy++) {
        mvaddch(y + dy, x, '|');
        mvaddch(y + dy, x + CELL_W - 1, '|');
    }
    mvaddch(y, x, '+');
    mvaddch(y, x + CELL_W - 1, '+');
    mvaddch(y + CELL_H - 1, x, '+');
    mvaddch(y + CELL_H - 1, x + CELL_W - 1, '+');

    if (letter) {
        if (g_has_color) attron(COLOR_PAIR(pair) | A_BOLD);
        mvprintw(y + CELL_H / 2, x + 1, " %c ", letter);
        attrset(A_NORMAL);
    }
}

static void _draw_guess_row(int row, const char* letters, const LetterState* states) {
    int start_x = 4;
    int start_y = 3 + row * (CELL_H + 1);

    for (int i = 0; i < WORDLE_WORD_LEN; i++)
        _draw_cell(start_y, start_x + i * (CELL_W + 1), letters[i], states[i]);
}

static void _draw_keyboard(const WordleGame* g) {
    const char* rows[] = { "QWERTYUIOP", "ASDFGHJKL", "ZXCVBNM" };
    int board_bottom;
    if (g) {
        int n = wordle_guess_count(g);
        board_bottom = 3 + n * (CELL_H + 1) + CELL_H + 2;
    } else {
        board_bottom = 3;
    }
    int kb_y = LINES - 4;
    if (kb_y <= board_bottom)
        kb_y = board_bottom + 2;
    if (kb_y + 3 > LINES)
        kb_y = LINES - 3;
    for (int r = 0; r < 3; r++) {
        for (int c = 0; rows[r][c]; c++) {
            char ch = rows[r][c];
            char up = (char)toupper((unsigned char)ch);
            LetterState best = LS_ABSENT;
            bool was_guessed = false;
            for (int i = 0; i < wordle_guess_count(g); i++) {
                const GuessResult* gr = wordle_get_result(g, i);
                if (!gr) continue;
                for (int j = 0; j < WORDLE_WORD_LEN; j++) {
                    if (gr->letters[j] == up) {
                        was_guessed = true;
                        if (gr->states[j] > best)
                            best = gr->states[j];
                    }
                }
            }
            if (was_guessed) {
                int pair = (best == LS_CORRECT) ? C_GREEN : C_YELLOW;
                if (best == LS_ABSENT) pair = C_BLUE;
                if (g_has_color) attron(COLOR_PAIR(pair) | A_BOLD);
            } else {
                attrset(A_NORMAL);
            }
            mvprintw(kb_y + r, 4 + c * 2, "%c", up);
            attrset(A_NORMAL);
        }
    }
}

static void _draw_screen(const WordleGame* g, const char* input_buf, int input_len, const char* msg) {
    clear();
    int mx, my;
    getmaxyx(stdscr, my, mx);
    attrset(A_NORMAL);
    if (g_has_color) attron(COLOR_PAIR(C_GREEN) | A_BOLD);
    const char* title = "W O R D L E";
    mvprintw(0, (mx - (int)strlen(title)) / 2, "%s", title);
    attrset(A_NORMAL);

    if (!g) { refresh(); return; }
    for (int i = 0; i < wordle_guess_count(g); i++) {
        const GuessResult* r = wordle_get_result(g, i);
        if (!r) continue;
        _draw_guess_row(i, r->letters, r->states);
    }
    if (!wordle_is_over(g)
        && wordle_guess_count(g) < wordle_total_attempts(g)) {

        char letters[WORDLE_WORD_LEN + 1] = {0};
        LetterState states[WORDLE_WORD_LEN];
        for (int i = 0; i < WORDLE_WORD_LEN; i++) {
            letters[i] = (i < input_len) ? input_buf[i] : '\0';
            states[i]  = LS_ABSENT;
        }
        _draw_guess_row(wordle_guess_count(g), letters, states);
    }
    attrset(A_NORMAL);
    char buf[64];
    snprintf(buf, sizeof(buf),
             "Attempts left: %d   Guesses: %d / %d",
             wordle_attempts_left(g),
             wordle_guess_count(g),
             wordle_total_attempts(g));
    mvprintw(my - 6, 4, "%s", buf);
    if (msg && msg[0]) {
        if (g_has_color) attron(COLOR_PAIR(C_YELLOW) | A_BOLD);
        mvprintw(my - 5, 4, "%s", msg);
        clrtoeol();
        attrset(A_NORMAL);
    }
    if (wordle_is_over(g)) {
        char result[128];
        if (wordle_is_won(g))
            snprintf(result, sizeof(result), "YOU WON!  Word: %s  (%d guesses)", wordle_target(g), wordle_guess_count(g));
        else
            snprintf(result, sizeof(result), "GAME OVER.  The word was: %s", wordle_target(g));

        if (g_has_color) attron(COLOR_PAIR(C_YELLOW) | A_BOLD);
        mvprintw(my / 2, (mx - (int)strlen(result)) / 2, "%s", result);
        attrset(A_NORMAL);
    }

    if (!wordle_is_over(g))
        _draw_keyboard(g);
    attrset(A_NORMAL);
    mvprintw(my - 1, 0, "Q=quit  ENTER=submit  BACKSPACE=delete");
    refresh();
}

int wordle_ui_run(void) {
    initscr();
    cbreak();
    noecho();
    curs_set(1);
    keypad(stdscr, TRUE);
    _init_colors();

    int attempts = 6;
    char dict_file[256];
    int mx, my;
    snprintf(dict_file, sizeof(dict_file), "%s", WORDLE_DICT_FILE);

    while (1) {
        clear();
        getmaxyx(stdscr, my, mx);
        attrset(A_NORMAL);

        const char* title = "=== WORDLE ===";
        mvprintw(my / 2 - 5, (mx - (int)strlen(title)) / 2, "%s", title);
        mvprintw(my / 2 - 2, (mx - 35) / 2, "1) New game (default 6 attempts)");
        mvprintw(my / 2 - 1, (mx - 35) / 2, "2) New game (custom attempts)");
        mvprintw(my / 2,     (mx - 35) / 2, "3) Settings (dictionary file)");
        mvprintw(my / 2 + 1, (mx - 35) / 2, "Q) Quit");
        refresh();

        int ch = getch();
        if (ch == 'q' || ch == 'Q' || ch == 27) { endwin(); return 0; }

        if (ch == '2') {
            echo(); curs_set(1);
            char buf[32] = "";
            mvprintw(my / 2 + 3, (mx - 30) / 2, "Attempts (1-99): ");
            clrtoeol(); refresh();
            mvgetnstr(my / 2 + 3, (mx - 30) / 2 + 19, buf, sizeof(buf) - 1);
            attempts = atoi(buf);
            if (attempts < 1 || attempts > 99) attempts = 6;
            noecho();
        } else if (ch == '3') {
            echo(); curs_set(1);
            char buf2[256] = "";
            mvprintw(my / 2 + 3, (mx - 50) / 2, "Dict [%s]: ", dict_file);
            clrtoeol(); refresh();
            mvgetnstr(my / 2 + 3, (mx - 50) / 2 + 12, buf2, sizeof(buf2) - 1);
            if (buf2[0] != '\0')
                strncpy(dict_file, buf2, sizeof(dict_file) - 1);
            noecho();
            continue;
        } else if (ch != '1') {
            continue;
        }
        break;
    }

    WordleGame* g = wordle_create(attempts, dict_file);
    if (!g) {
        endwin();
        fprintf(stderr, "Error: cannot load dictionary '%s'\n", dict_file);
        return -1;
    }

    char input_buf[WORDLE_WORD_LEN + 1] = {0};
    int  input_len = 0;
    const char* msg = "";

    while (!wordle_is_over(g)) {
        _draw_screen(g, input_buf, input_len, msg);
        msg = "";

        int ch = getch();

        if (ch == 'q' || ch == 'Q' || ch == 27) {
            wordle_destroy(g);
            endwin();
            return 0;
        }
        else if (ch == KEY_BACKSPACE || ch == 127 || ch == 8) {
            if (input_len > 0) input_len--;
            input_buf[input_len] = '\0';
        }
        else if (ch == '\n' || ch == KEY_ENTER) {
            if (input_len != WORDLE_WORD_LEN) {
                msg = "Not enough letters";
                continue;
            }
            int rc = wordle_try_guess(g, input_buf);
            if (rc == -1) msg = "Not in word list";
            else if (rc == 1) msg = "Correct! You won!";
            else if (rc == -2) msg = "No more attempts";
            input_len = 0;
            input_buf[0] = '\0';
        }
        else if (isalpha(ch)) {
            if (input_len < WORDLE_WORD_LEN) {
                input_buf[input_len++] = (char)toupper(ch);
                input_buf[input_len] = '\0';
            }
        }
    }

    _draw_screen(g, "", 0, "");
    attrset(A_NORMAL);
    mvprintw(my - 2, 0, "Press any key to quit...");
    refresh();
    timeout(-1);
    getch();

    wordle_destroy(g);
    endwin();
    return 0;
}
