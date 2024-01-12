# A simple Makefile for Project 2 of COMP30023. By Max Besley.

CC=gcc
CFLAGS=-Wall

SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)

OBJ_DIR=obj
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

BIN_DIR=bin
EXE_NAME=server
EXE=$(BIN_DIR)/$(EXE_NAME)


all: $(EXE) clean

$(EXE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "-------------- EXECUTABLE \"$(EXE_NAME)\" CREATED --------------"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@echo "Deleting object files..."
	rm -f $(OBJ_DIR)/*.o
	rmdir $(OBJ_DIR)
	@echo "------------------- CLEAN UP COMPLETE -------------------"

format:
	clang-format -i *.c *.h

.PHONY: clean format
