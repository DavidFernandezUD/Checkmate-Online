# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Ofast -Wno-unused

# Directories
SRC_DIR = src
BIN_DIR = bin

# Targets
all: chess

chess: $(BIN_DIR)/chess

$(BIN_DIR)/chess: $(SRC_DIR)/main.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -lm -o $@

server: $(BIN_DIR)/server

$(BIN_DIR)/server: $(SRC_DIR)/server/server.c $(wildcard $(SRC_DIR)/server/*.c)
	$(CC) $(CFLAGS) $^ -lm -o $@

# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all chess clean
