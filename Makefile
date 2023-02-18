C_COMPILER = gcc
C_FLAGS = -Wall -Wextra -Werror -pedantic -std=c17
TARGET = copy

all: $(TARGET).c
	$(C_COMPILER) $(C_FLAGS) -o $(TARGET) $(TARGET).c