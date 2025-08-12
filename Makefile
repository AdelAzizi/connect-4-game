# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Isrc
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system

# Target executable name
TARGET = connect4

# Source directories
PLAYER_DIR = src/Player
GAMEBOARD_DIR = src/GameBoard
GAME_DIR = src/ConnectFourGame

# Find all .cpp files in src and its subdirectories
SOURCES = $(wildcard src/**/*.cpp) $(wildcard src/*.cpp)

# Object files directory
BUILD_DIR = build
# Create a corresponding .o file in the build directory for each source file
OBJECTS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SOURCES))

# Default target
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJECTS)
	@echo "Linking..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(LDFLAGS)

# Rule to compile source files into object files
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(@D)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build artifacts
clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR) $(TARGET)

# Phony targets
.PHONY: all clean
