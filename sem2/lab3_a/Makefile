BUILD_DIR = build_table
TARGET = prog3
LIB_NAME = libtable.a

CC = gcc
CFLAGS = -Wextra -Wall -Werror -g
VGFLAGS = --leak-check=full --show-leak-kinds=all --track-origins=yes -s --suppressions=readline.supp
AR = ar
ARFLAGS = rcs

LIB_SRCS = table.c input.c dial.c
LIB_OBJS = $(LIB_SRCS:%.c=$(BUILD_DIR)/%.o)

MAIN_SRC = prog3.c
MAIN_OBJ = $(BUILD_DIR)/prog3.o

.PHONY: all build lib run clean

all: build

lib: $(BUILD_DIR) $(BUILD_DIR)/$(LIB_NAME)

$(BUILD_DIR)/$(LIB_NAME): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $^

build: $(BUILD_DIR) lib $(MAIN_OBJ)
	$(CC) $(MAIN_OBJ) -L$(BUILD_DIR) -lreadline -ltable -o $(BUILD_DIR)/$(TARGET) $(CFLAGS)

$(BUILD_DIR)/%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

run: build
	valgrind $(VGFLAGS) ./$(BUILD_DIR)/$(TARGET)

clean:
	rm -rf $(BUILD_DIR)

