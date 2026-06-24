CC = gcc
CFLAGS = -Wall -Wextra -fPIC -g
LDFLAGS = -lncurses
VALGRIND = valgrind --leak-check=full --suppressions=valgrind.supp --error-exitcode=1
INC_DIR = include
SRC_DIR = src
OBJ_DIR = obj
INCS = $(INC_DIR)/dict.h $(INC_DIR)/hash_table.h $(INC_DIR)/rb_tree.h $(INC_DIR)/wordle.h $(INC_DIR)/wordle_ui.h

HT_LIB = libdict_ht.so
RB_LIB = libdict_rb.so

.PHONY: all ht rb clean valgrind valgrind_ht valgrind_rb test run run_rb

all: ht

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(HT_LIB): obj/hash_table_ht.o obj/dict_ht.o
	$(CC) -shared -o $@ $^
	@echo "Собрана $(HT_LIB)"

obj/hash_table_ht.o: $(SRC_DIR)/hash_table.c $(INC_DIR)/hash_table.h $(INC_DIR)/dict.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DUSE_RB_TREE=0 -I$(INC_DIR) -c $< -o $@

obj/dict_ht.o: $(SRC_DIR)/dict.c $(INCS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DUSE_RB_TREE=0 -I$(INC_DIR) -c $< -o $@

wordle_ht: $(HT_LIB) $(SRC_DIR)/wordle.c $(SRC_DIR)/wordle_ui.c $(SRC_DIR)/main.c $(INCS)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(SRC_DIR)/wordle.c $(SRC_DIR)/wordle_ui.c $(SRC_DIR)/main.c -L. -ldict_ht -o $@ $(LDFLAGS)
	@echo "Собран wordle_ht"

ht: wordle_ht
	@echo "=== Собрана версия с ХЕШ-ТАБЛИЦЕЙ ==="

$(RB_LIB): obj/rb_tree_rb.o obj/dict_rb.o
	$(CC) -shared -o $@ $^
	@echo "Собрана $(RB_LIB)"

obj/rb_tree_rb.o: $(SRC_DIR)/rb_tree.c $(INC_DIR)/rb_tree.h $(INC_DIR)/dict.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DUSE_RB_TREE=1 -I$(INC_DIR) -c $< -o $@

obj/dict_rb.o: $(SRC_DIR)/dict.c $(INCS) | $(OBJ_DIR)
	$(CC) $(CFLAGS) -DUSE_RB_TREE=1 -I$(INC_DIR) -c $< -o $@

wordle_rb: $(RB_LIB) $(SRC_DIR)/wordle.c $(SRC_DIR)/wordle_ui.c $(SRC_DIR)/main.c $(INCS)
	$(CC) $(CFLAGS) -I$(INC_DIR) $(SRC_DIR)/wordle.c $(SRC_DIR)/wordle_ui.c $(SRC_DIR)/main.c -L. -ldict_rb -o $@ $(LDFLAGS)
	@echo "Собран wordle_rb"

rb: wordle_rb
	@echo "=== Собрана версия с RB-ДЕРЕВОМ ==="

run: wordle_ht
	LD_LIBRARY_PATH=. ./wordle_ht

run_rb: wordle_rb
	LD_LIBRARY_PATH=. ./wordle_rb

valgrind_ht: wordle_ht
	LD_LIBRARY_PATH=. $(VALGRIND) ./wordle_ht

valgrind_rb: wordle_rb
	LD_LIBRARY_PATH=. $(VALGRIND) ./wordle_rb

valgrind: valgrind_ht

clean:
	rm -rf $(OBJ_DIR) $(HT_LIB) $(RB_LIB) wordle_ht wordle_rb
	@echo "Очищено"

test: clean wordle_ht wordle_rb
	@echo "=== Обе версии собраны успешно ==="
