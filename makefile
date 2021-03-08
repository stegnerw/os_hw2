# File			: Makefile
# Author		: Wayne Stegner <wayne.stegner@protonmail.com>

# Project Name
PROJECT = os_hw2

# Directory Definitions
# src/	- Source code directory
# bin/	- Compiled executable directory
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
	$(CC) $(CFLAGS) -o $@ $^ -lpthread -lrt

$(BIN_DIR):
	mkdir -p $@

clean:
	rm -rf $(BIN_DIR)

