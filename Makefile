C_COMPILER = gcc
C_FLAGS = -Wall -Wextra -Werror -pedantic -std=c99 -g

all: $(TARGET)
	$(C_COMPILER) $(C_FLAGS) -o ./build/$(TARGET) $(TARGET).c