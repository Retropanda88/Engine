# Nombre del ejecutable
TARGET = bin

# Directorios
SRC_DIR = src
INC_DIR = include
BUILD_DIR = build

# Archivos fuente
C_SRCS := $(wildcard $(SRC_DIR)/*.c)
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(C_SRCS:$(SRC_DIR)/%.c=$(BUILD_DIR)/%.o) \
        $(CPP_SRCS:$(SRC_DIR)/%.cpp=$(BUILD_DIR)/%.o)

# Compiladores
CC = gcc
CXX = g++
CFLAGS = -O3  -Wall -I$(INC_DIR) `sdl-config --cflags`
CXXFLAGS = -O3 -Wall -I$(INC_DIR) `sdl-config --cflags`
LDFLAGS = `sdl-config --libs`  -lSDL_image -lSDL_mixer -lSDL -lpng -ljpeg

# Regla principal
all: $(BUILD_DIR)/$(TARGET)

# Vinculación final
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compilación de .c
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilación de .cpp
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean