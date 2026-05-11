BUILD_DIR = build_tree
TARGET = prog4
LIB_NAME = libtree.a

CC = gcc
CFLAGS = -Wextra -Wall -Werror -g -std=c11
VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -s --suppressions=readline.supp
AR = ar
ARFLAGS = rcs

LIB_SRCS = tree.c input.c
LIB_OBJS = $(LIB_SRCS:%.c=$(BUILD_DIR)/%.o)

APP_SRCS = prog4.c dial.c
APP_OBJS = $(APP_SRCS:%.c=$(BUILD_DIR)/%.o)

TIMER_SRC = timer.c
TIMER_OBJ = $(BUILD_DIR)/timer.o
TIMER_TARGET = timer

.PHONY: all build lib run clean asan timer

all: build

lib: $(BUILD_DIR) $(BUILD_DIR)/$(LIB_NAME)

$(BUILD_DIR)/$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

build: $(BUILD_DIR) lib $(APP_OBJS)
	$(CC) $(APP_OBJS) -L$(BUILD_DIR) -LC:/msys64/mingw64/lib -ltree -lreadline -o $(BUILD_DIR)/$(TARGET) $(CFLAGS)

$(BUILD_DIR)/$(TIMER_OBJ): $(TIMER_SRC)
	$(CC) $(CFLAGS) -c $< -o $@

timer: $(BUILD_DIR) lib $(TIMER_OBJ)
	$(CC) $(TIMER_OBJ) -L$(BUILD_DIR) -ltree -o $(BUILD_DIR)/$(TIMER_TARGET) $(CFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: build
	valgrind $(VGFLAGS) ./$(BUILD_DIR)/$(TARGET)

asan: CFLAGS += -fsanitize=address,undefined
asan: build

clean:
	rm -rf $(BUILD_DIR)