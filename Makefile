CC = gcc
CFLAGS = -Wall -Wextra -std=c11
REFRIGERANT = -DREFR_=REFR_R290

LIB_SRCS = $(wildcard $(SRC_DIR)/*.c)
LIB_OBJS = $(patsubst $(SRC_DIR)/%.c, $(SRC_DIR)/%.o, $(LIB_SRCS))
LIB_NAME = thermo

EXAMPLES_SRCS = $(wildcard $(EXAMPLES_DIR)/*.c)
EXAMPLE_OBJS = $(patsubst $(EXAMPLES_DIR)/%.c, $(EXAMPLES_DIR)/%.o, $(EXAMPLES_SRCS))
EXAMPLE_BINS = $(patsubst $(EXAMPLES_DIR)/%.c, $(EXAMPLES_DIR)/build/%.exe, $(EXAMPLES_SRCS))

TESTS_SRCS = $(wildcard $(TESTS_DIR)/*.c)
TEST_OBJS = $(patsubst $(TESTS_DIR)/%.c, $(TESTS_DIR)/%.o, $(TESTS_SRCS))
TEST_BINS = $(patsubst $(TESTS_DIR)/%.c, $(TESTS_DIR)/build/%.exe, $(TESTS_SRCS))

LIB_DIR = lib
SRC_DIR = src
EXAMPLES_DIR = examples
TESTS_DIR = tests
INCLUDE_DIR = include

all: lib examples tests

lib: $(LIB_DIR)/lib$(LIB_NAME).a

$(LIB_DIR)/lib$(LIB_NAME).a: $(LIB_OBJS)
	ar rcs $@ $^

$(SRC_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR) $(REFRIGERANT)

examples: $(EXAMPLE_BINS)

$(EXAMPLES_DIR)/build/%.exe: $(EXAMPLES_DIR)/%.o $(LIB_DIR)/lib$(LIB_NAME).a
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -l$(LIB_NAME) 

$(EXAMPLES_DIR)/%.o: $(EXAMPLES_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

tests: $(TEST_BINS) $(TEST_OBJS)

$(TESTS_DIR)/build/%.exe: $(TESTS_DIR)/%.o $(LIB_DIR)/lib$(LIB_NAME).a
	$(CC) $(CFLAGS) $< -o $@ -L$(LIB_DIR) -l$(LIB_NAME)

$(TESTS_DIR)/%.o: $(TESTS_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@ -I$(INCLUDE_DIR)

clean:
	del /Q $(LIB_DIR)\*.o $(LIB_DIR)\*.a $(EXAMPLES_DIR)\build\*.exe
