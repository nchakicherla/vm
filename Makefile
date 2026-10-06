CC      ?= cc
CFLAGS  ?= -std=c11 -Wall -Wextra -pedantic -g
TARGET  := vm
SRC     := src
BUILD   := build

SRCS := $(wildcard $(SRC)/*.c)
OBJS := $(SRCS:$(SRC)/%.c=$(BUILD)/%.o)
DEPS := $(OBJS:.o=.d)

.PHONY: all run valgrind clean

all: $(BUILD)/$(TARGET)

$(BUILD)/$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILD)/%.o: $(SRC)/%.c | $(BUILD)
	$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD):
	mkdir -p $@

run: all
	./$(BUILD)/$(TARGET)

valgrind: all
	valgrind --leak-check=full --error-exitcode=1 ./$(BUILD)/$(TARGET)

clean:
	rm -rf $(BUILD)

-include $(DEPS)
