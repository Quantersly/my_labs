.PHONY: all build_stack build_vector run clean

BUILD_DIR = game_build
TARGET = prog1
CC = gcc
CFLAGS = -Wextra -Wall -Werror -g
LDFLAGS = -lncursesw
VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -s --suppressions=ncurses.supp

ALL_SRCS := $(wildcard *.c)
EXCLUDE := log_game_stack.c log_game_vector.c
CORE_SRCS := $(filter-out $(EXCLUDE), $(ALL_SRCS))

all: build_queue run

build_stack:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CORE_SRCS) log_game_stack.c -o $(BUILD_DIR)/$(TARGET) $(CFLAGS) $(LDFLAGS)

build_vector:
	mkdir -p $(BUILD_DIR)
	$(CC) $(CORE_SRCS) log_game_vector.c -o $(BUILD_DIR)/$(TARGET) $(CFLAGS) -DUSE_VECTOR $(LDFLAGS)

run:
	valgrind $(VGFLAGS) $(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

