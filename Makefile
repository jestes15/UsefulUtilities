C_COMPILER = clang
C_FLAGS = -pthread -I/usr/include -std=gnu17
TARGET = copy

all: $(TARGET).c
	$(C_COMPILER) $(C_FLAGS) -o $(TARGET) $(TARGET).c