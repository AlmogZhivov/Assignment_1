CC = g++
CFLAGS = -std=c++11 -Wall

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

# List of source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)

# Create a list of object files by replacing .cpp with .o
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Target executable
TARGET = $(BIN_DIR)/my_program

# Build executable
$(TARGET): $(OBJ_FILES)
	$(CC) $(CFLAGS) $^ -o $@

# Build object files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Create necessary directories
$(shell mkdir -p $(OBJ_DIR) $(BIN_DIR))

# Clean up intermediate and executable files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Phony target to avoid conflicts with file named "clean"
.PHONY: clean