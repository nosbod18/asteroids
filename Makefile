# Simple Makefile that works for OpenGL and SDL2

CC := clang

SRC_DIR := src
BIN_DIR := bin
OBJ_DIR := $(BIN_DIR)/obj

SRC_EXT := c

BIN := $(BIN_DIR)/game

SRCS := $(shell find $(SRC_DIR) -type f -name *.$(SRC_EXT))
INCS := $(shell find $(SRC_DIR) -type d)
OBJS := $(SRCS:%.$(SRC_EXT)=$(OBJ_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

OGL_FLAGS := -lglfw -lglew -framework OpenGL
SDL_FLAGS := -lsdl2 -lsdl2_gfx

INC_FLAGS := $(addprefix -I,$(INCS))
CFLAGS := -g -Wall -Wextra -Wno-unused-parameter -Wno-missing-braces $(INC_FLAGS) -MMD -MP

LDFLAGS := -lm $(SDL_FLAGS)

.PHONY: all build clean

all: $(BIN)
	@$(BIN)

build: $(BIN)

$(BIN): $(OBJS)
	@$(CC) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: %.$(SRC_EXT)
	@mkdir -p $(@D)
	@$(CC) -c -o $@ $< $(CFLAGS)

clean:
	@rm -r $(BIN_DIR)/*

-include $(DEPS)