C_COMPILER = gcc
C_FLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g

# Path: Makefile
all: $(TARGET)
	$(C_COMPILER) $(C_FLAGS) -o $(TARGET).o $(TARGET).c