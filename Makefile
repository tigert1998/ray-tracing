CPP_FLAG = -std=c++17
OPT_FLAG = -O2
CC = clang++
SRC_DIR = src
INCLUDE_DIR = include
OBJ_DIR = obj
BIN_DIR = bin
INCLUDE_FLAG = -I$(INCLUDE_DIR)
SRC_FILES = $(wildcard $(SRC_DIR)/*)
OBJ_FILES = $(patsubst %.cpp, $(OBJ_DIR)/%.o, $(notdir $(SRC_FILES)))
TARGET_FILES = $(BIN_DIR)/main

all: $(TARGET_FILES)

$(TARGET_FILES): $(OBJ_FILES)
	$(CC) $(OPT_FLAG) $(CPP_FLAG) $(OBJ_FILES) -o $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	-$(CC) $(CPP_FLAG) $(INCLUDE_FLAG) -c $< -o $@

clean:
	-rm $(OBJ_FILES)

