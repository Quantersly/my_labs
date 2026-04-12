.PHONY: all build run
BUILD_DIR=build
TARGET=prog_4
CC=gcc
CFLAGS=-Wextra -Wall -Werror -g -lreadline
VGFLAGS=--leak-check=full --show-leak-kinds=all --track-origins=yes -s --suppressions=readline.supp
OBJECTS:=$(wildcard *.c)

all: build run

build:
	mkdir -p $(BUILD_DIR)
	$(CC) $(OBJECTS) -o $(BUILD_DIR)/$(TARGET) $(CFLAGS)

run:
	valgrind $(VGFLAGS) $(BUILD_DIR)/$(TARGET)
