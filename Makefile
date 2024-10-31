SRC_DIR=src
INC_DIR=include
OBJ_DIR=obj
BIN_DIR=bin

CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -Werror
CFLAGS += -Wno-unused-parameter
CFLAGS += -I$(INC_DIR)

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN_DIR)/rv32ima: $(OBJECTS) | $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR):
	mkdir -p $@
$(BIN_DIR):
	mkdir -p $@

.PHONY: clean test

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

test: $(BIN_DIR)/rv32ima
	./$(BIN_DIR)/rv32ima
