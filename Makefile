# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -oFast -Wno-unused

# Directories
SRC_DIR = src
BIN_DIR = bin

# Targets
all: $(BIN_DIR)/main

$(BIN_DIR)/main: $(SRC_DIR)/main.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -lm -o $@

# Clean
clean:
	rm -rf $(BIN_DIR)

