C_COMPILER = clang
C_FLAGS = --std=c17 -lrt -pthread -I/usr/include
TARGET = copy

all: $(TARGET).c
	$(C_COMPILER) $(C_FLAGS) -o $(TARGET) $(TARGET).c