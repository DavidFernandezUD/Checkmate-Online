# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c99 -Ofast -Wno-unused

# Platform-specific settings
ifeq ($(OS),Windows_NT)
    # Windows specific settings
    CFLAGS += -DWINDOWS
else
    # Linux specific settings
    CFLAGS += -DLINUX
endif

# Directories
SRC_DIR  = src
BIN_DIR  = bin
TEST_DIR = test
LIB_DIR  = lib

# Targets
all: chess server


# Chess
chess: $(BIN_DIR)/chess

$(BIN_DIR)/chess: $(wildcard $(SRC_DIR)/chess/*.c) $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) $^ -lm -o $@


# Chess profiling
chess_profile: $(BIN_DIR)/chess_profile

$(BIN_DIR)/chess_profile: $(SRC_DIR)/main.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -lm -pg -o $@


# Tests
test: $(BIN_DIR)/test

$(BIN_DIR)/test: $(TEST_DIR)/movegen_test.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -o $@

	./$(BIN_DIR)/test


# Server
server: $(BIN_DIR)/server

$(BIN_DIR)/server: $(SRC_DIR)/server/server_main.c $(wildcard $(SRC_DIR)/server/*.c) $(LIB_DIR)/sqlite/sqlite3.c
	$(CC) $^ -lm -o $@


# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all chess server clean
