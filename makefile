# File			: Makefile
# Author		: Wayne Stegner <wayne.stegner@protonmail.com>

# Project Name
PROJECT = compiler

# Directory Definitions
# src/	- Source code directory
# obj/	- Compiled object directory
# bin/	- Compiled executable directory
# test/	- Test case directory
# log/	- Test log directory
SRC_DIR		= ./src
BIN_DIR		= ./bin

# Tell Make which shell to use
SHELL 		= bash

# Compiler Info
CC 			= g++
CFLAGS 		= -g -Wall

# Build Targets
.PHONY: clean

all: $(BIN_DIR)/cigarette_smokers $(BIN_DIR)/sleeping_barber

$(BIN_DIR)/cigarette_smokers: $(SRC_DIR)/cigarette_smokers.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR)/sleeping_barber: $(SRC_DIR)/sleeping_barber.cpp | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)
