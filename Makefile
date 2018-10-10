CPP_FLAG = -std=c++17
OPT_FLAG = -O2
CC = clang++
SRC_DIR = src
INCLUDE_DIRS = $(shell find include -maxdepth 1 -type d)
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_FLAG = $(foreach i, $(INCLUDE_DIRS), -I$(i))
SRC_FILES = $(shell find $(SRC_DIR) -maxdepth 2 -type f)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))
TARGET_FILES = $(BIN_DIR)/main

DIRS = $(OBJ_DIR) $(BIN_DIR)


all: $(DIRS) $(TARGET_FILES)

$(DIRS):
	mkdir $@

$(TARGET_FILES): $(OBJ_FILES)
	$(CC) $(OPT_FLAG) $(CPP_FLAG) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@if [ ! -d $(dir $@) ]; then mkdir $(dir $@); fi
	-$(CC) $(CPP_FLAG) $(INCLUDE_FLAG) -c $< -o $@

clean:
	-rm -r $(OBJ_DIR)/*
	-rm -r $(BIN_DIR)/*

