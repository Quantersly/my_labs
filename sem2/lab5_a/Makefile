CC = gcc
CFLAGS = -Wall -Wextra -pedantic -std=c11 -g -finput-charset=utf-8
LDLIBS = -lreadline

BUILD_DIR = build_lab5

LIB_SRC = graph.c
LIB_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(LIB_SRC))
LIB_TARGET = $(BUILD_DIR)/libgraph.a

APP_SRC = prog5.c dial.c input.c
APP_OBJ = $(patsubst %.c,$(BUILD_DIR)/%.o,$(APP_SRC))
TARGET = $(BUILD_DIR)/prog5

all: lib build run

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

lib: $(BUILD_DIR) $(LIB_TARGET)

$(LIB_TARGET): $(LIB_OBJ)
	ar rcs $@ $^

build: lib $(APP_OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(APP_OBJ) $(LIB_TARGET) $(LDLIBS)

run: build
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes -s --suppressions=readline.supp $(TARGET)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean lib build run
