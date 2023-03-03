C_COMPILER = clang++
C_FLAGS = -I/usr/include -std=c++17
TARGET = generate_timestamp

all: $(TARGET).cpp
	$(C_COMPILER) $(C_FLAGS) -o ../bin/$(TARGET) $(TARGET).cpp