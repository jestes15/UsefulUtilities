C_COMPILER = clang
C_FLAGS = -I/usr/include -std=c17
TARGET = generate_timestamp

all: $(TARGET).c
	$(C_COMPILER) $(C_FLAGS) -o $(TARGET) $(TARGET).c