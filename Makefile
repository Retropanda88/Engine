# Nombre del ejecutable
TARGET = bin

# Directorios
SRC_DIR = src
LIBMAD_SRC_DIR = libmad/src
LIBMAD_INC_DIR = libmad/include
INC_DIR = include
BUILD_DIR = build

# Archivos fuente
C_SRCS := $(wildcard $(SRC_DIR)/*.c) \
          $(wildcard $(LIBMAD_SRC_DIR)/*.c)
CPP_SRCS := $(wildcard $(SRC_DIR)/*.cpp)
OBJS := $(C_SRCS:%.c=$(BUILD_DIR)/%.o) \
        $(CPP_SRCS:%.cpp=$(BUILD_DIR)/%.o)

# Compiladores
CC = gcc
CXX = g++
CFLAGS = -O3 -Wall  -DHAVE_CONFIG_H -DHAVE_SYS_TYPES_H -DHAVE_ERRNO_H -DHAVE_UNISTD_H -I$(INC_DIR) -I$(LIBMAD_INC_DIR) `sdl-config --cflags`
CXXFLAGS = -O3 -Wall -I$(INC_DIR) -I$(LIBMAD_INC_DIR) `sdl-config --cflags`
LDFLAGS = `sdl-config --libs` -lSDL_image -lSDL_mixer -lSDL -lpng -ljpeg

# Regla principal
all: $(BUILD_DIR)/$(TARGET)

# Vinculación final
$(BUILD_DIR)/$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDFLAGS)

# Compilación de .c
$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilación de .cpp
$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Limpieza
clean:
	rm -rf $(BUILD_DIR)/*

.PHONY: all clean