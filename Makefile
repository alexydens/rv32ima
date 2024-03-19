INC_DIR=./elm99/include/
SRC_DIR=src
BUILD_DIR=build

CFLAGS = -std=c99 -pedantic -Wall -Wextra -I$(INC_DIR)
LDFLAGS = -O3 -ffast-math -lm

elm99/build/libelm.a:
	cd elm99 && make build

$(BUILD_DIR)/main: $(SRC_DIR)/main.c elm99/build/libelm.a
	$(CC) $(SRC_DIR)/*.c elm99/build/libelm.a -o $@ $(CFLAGS) $(LDFLAGS)

.PHONY: clean test

clean:
	rm -rf $(BUILD_DIR)/*

test: $(BUILD_DIR)/main
	$(BUILD_DIR)/main
