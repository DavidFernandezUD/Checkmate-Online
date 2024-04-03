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

SQLITE_LIB = $(LIB_DIR)/sqlite/sqlite3.c


# Targets
all: chess server


# Chess
chess: $(BIN_DIR)/chess

$(BIN_DIR)/chess: $(SRC_DIR)/main.c $(wildcard $(SRC_DIR)/chess/*.c)
	$(CC) $(CFLAGS) $^ -lm -o $@


# Server
server: $(BIN_DIR)/server

$(BIN_DIR)/server: $(SRC_DIR)/server/serverMain.c $(wildcard $(SRC_DIR)/server/*.c) $(SQLITE_LIB)
	$(CC) $^ -lm -o $@


# Clean
clean:
	rm -rf $(BIN_DIR)

.PHONY: all chess clean
