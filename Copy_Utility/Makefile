C_COMPILER = clang
C_FLAGS = -pthread -I/usr/include -std=gnu17
TARGET = copy

all: $(TARGET).c
	mkdir ../bin
	$(C_COMPILER) $(C_FLAGS) -o ../bin/$(TARGET) $(TARGET).c