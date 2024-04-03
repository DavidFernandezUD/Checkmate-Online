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
SRC_DIR = src
BIN_DIR = bin
LIB_DIR = lib

# Targets
all: chess server

# Chess
chess: $(BIN_DIR)/chess

$(BIN_DIR)/chess: $(SRC_DIR)/main.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -lm -o $@

# Server
server: $(BIN_DIR)/server

$(BIN_DIR)/server: $(SRC_DIR)/server/server_main.c $(wildcard $(SRC_DIR)/server/*.c) $(LIB_DIR)/sqlite/sqlite3.o
	$(CC) $^ -lm -o $@

# Compile SQLite object file
$(LIB_DIR)/sqlite/sqlite3.o: $(wildcard $(LIB_DIR)/sqlite/sqlite3.c)
	$(CC) -c $^ -o $@ -I$(LIB_DIR)/sqlite

# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all chess server clean
