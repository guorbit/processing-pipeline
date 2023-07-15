# Define the compiler
CC := g++

# Define the source and build directories
SRC_DIR := ./
BUILD_DIR := build

# Objects and executable
OBJS := $(addprefix $(BUILD_DIR)/, main.o segfilter.o TFLiteModel.o)
TARGET := $(BUILD_DIR)/pipeline

# Compiler flags
CFLAGS := -Wall -Werror -Wpedantic

#  Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -ltensorflow -o $(TARGET) -lstdc++

# Object file rules
$(BUILD_DIR)/main.o: main.cpp ./src/filter/IFilter.hpp ./src/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c main.cpp -o $@

$(BUILD_DIR)/segfilter.o: ./src/filter/segfilter.cpp ./src/filter/segfilter.hpp $(BUILD_DIR)/TFLiteModel.o
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c ./src/filter/segfilter.cpp -o $@

$(BUILD_DIR)/TFLiteModel.o: ./src/model/TFLiteModel.cpp ./src/model/TFLiteModel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c ./src/model/TFLiteModel.cpp -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)