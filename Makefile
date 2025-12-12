CC := gcc
CCFLAGS := -std=c99 -g -Wall -Werror -Wextra
CCTESTFLAGS := -std=c99
CCLINKFLAGS := -lm

# Directories
CLIB := clib
SRC_DIR := $(CLIB)/src
INCLUDE_DIR := $(CLIB)/include
BUILD_DIR := $(CLIB)/build

TEST_DIR := $(CLIB)/test
TEST_INCLUDE_DIR := $(TEST_DIR)/test_include
TEST_SRC_DIR = $(TEST_DIR)/test_src
TEST_BUILD_DIR = $(TEST_DIR)/test_build
TEST_BIN = $(TEST_DIR)/test

BIN := scripts/c_modules/clib.so

# Source files
SRCS := $(wildcard $(SRC_DIR)/**/*.c) $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(SRCS))

TEST_SRCS := $(wildcard $(TEST_SRC_DIR)/**/*.c) $(wildcard $(TEST_SRC_DIR)/*.c)
TEST_OBJS := $(patsubst $(TEST_SRC_DIR)/%.c, $(TEST_BUILD_DIR)/%.o, $(TEST_SRCS)) $(filter-out %main.o, $(OBJS))

# $(info SRCS = $(SRCS))
# $(info OBJS = $(OBJS))
# $(info TEST_SRCS = $(TEST_SRCS))
# $(info TEST_OBJS = $(TEST_OBJS))

CCFLAGS := $(CCFLAGS) -I$(INCLUDE_DIR)
TEST_CCFLAGS := $(CCTESTFLAGS) -I$(INCLUDE_DIR) -I$(TEST_INCLUDE_DIR)

# default target
all: $(BIN)

build: $(BIN)

# Link object files to get final executable
$(BIN): $(OBJS)
	$(CC) -shared $(CCFLAGS) -o $@ $^ $(CCLINKFLAGS)

# Compile each source file into an object file
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c -fPIC $< -o $@

clean:
	rm -rf $(BUILD_DIR) $(BIN)
	rm -rf $(TEST_BUILD_DIR) $(TEST_BIN)

test: 
	@$(MAKE) build > /dev/null
	@$(MAKE) $(TEST_BIN)
	@./$(TEST_BIN)
	@rm -rf $(TEST_BUILD_DIR) $(TEST_BIN)

$(TEST_BIN): $(TEST_OBJS)
	@$(CC) $(TEST_CCFLAGS) -o $@ $^ $(CCLINKFLAGS)

$(TEST_BUILD_DIR)/%.o: $(TEST_SRC_DIR)/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(TEST_CCFLAGS) -c $< -o $@