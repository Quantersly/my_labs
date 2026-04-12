#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <locale.h>

#include "game.h"
#include "log_game.h"


void handle_sigint(int sig) {
    printf("\n%d\n", sig);
    endwin();
    exit(0);
}

int main() {
    setlocale(LC_ALL, "");
    signal(SIGINT, handle_sigint);
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    int *array;
    Stack *log;
    int void_cell = 15, flag = 1;
    Err_tag res = ERR_TAG_OK;
    while(flag) {
        clear();
        refresh();
        printw("Выберите режим:\n");
        printw("1. Обычный\n");
        printw("2. Тестовый\n");
        int ch = getch();
        switch(ch) {
            case '1':                    
                res = start_game(&array, &log);
                flag = 0;
                break;
            case '2':
                res = test_game(&array, &log);
                flag = 0;
                void_cell = 10;
                break;
            case 'q':
                endwin();
                return 0;
        }

    }
    if (res != ERR_TAG_OK) {
        printw("\nОшибка при работе с памятью");
        return 0;
    }
    clear();
    print_tag(array);
    flag = 1;
    while(flag) {
        int ch = getch();
        switch(ch) {
            case KEY_UP:
                if (void_cell + 4 < 16) {
                    res = swap_cell(array, void_cell, void_cell + 4, log);
                    void_cell += 4;
                }
                break;
            case KEY_DOWN:
                if (void_cell - 4 > -1) {
                    res = swap_cell(array, void_cell, void_cell - 4, log);
                    void_cell -= 4;
                }
                break;
            case KEY_RIGHT:
                if ((void_cell - 1) % 4 != 3) {
                    res = swap_cell(array, void_cell, void_cell - 1, log);
                    void_cell--;
                }
                break;
            case KEY_LEFT:
                if ((void_cell + 1) % 4 != 0) {
                    res = swap_cell(array, void_cell, void_cell + 1, log);
                    void_cell++;
                }
                break;
            case 'o':
                pop_node(log, &array, &void_cell);
                break;
            case 'q':
                flag = 0;
                break;
        }
        if (res != ERR_TAG_OK) {
            erase_stack(log);
            printw("\nОшибка при работе с памятью");
            return 0;
        }
        print_tag(array);
        if (array[16] == 15) {
            printw("Вы победили!\n");
            getch();
            break;
        }
    }
    free(array);
    erase_stack(log);
    endwin();
    return 0;
}
