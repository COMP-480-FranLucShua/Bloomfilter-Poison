CC := gcc
CCFLAGS := -std=c99 -g -Wall -Werror -Wextra
CCLINKFLAGS := -lm

# Directories
CLIB := clib
SRC_DIR := $(CLIB)/src
INCLUDE_DIR := $(CLIB)/include
BUILD_DIR := $(CLIB)/build

BIN := test

# Source files
SRCS := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

CCFLAGS := $(CCFLAGS) -I$(INCLUDE_DIR)

# default target
all: $(BIN)

build: $(BIN)

# Link object files to get final executable
$(BIN): $(OBJS)
	$(CC) $(CCFLAGS) -o $@ $^ $(CCLINKFLAGS)

# Compile each source file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)