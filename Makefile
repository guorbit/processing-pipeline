# Define the compiler
CC := g++

# Define the source and build directories
SRC_DIR := ./src
BUILD_DIR := build

# Objects and executable
OBJS := $(addprefix $(BUILD_DIR)/, main.o segfilter.o TFLiteModel.o)
TARGET := $(BUILD_DIR)/pipeline

# Compiler flags
CFLAGS := -Wall -Werror -Wpedantic

# Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -ltensorflow -o $(TARGET) -lstdc++

# Object file rules
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/filter/IFilter.hpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/segfilter.o: $(SRC_DIR)/filter/segfilter.cpp $(SRC_DIR)/filter/segfilter.hpp $(BUILD_DIR)/TFLiteModel.o
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/filter/segfilter.cpp -o $@

$(BUILD_DIR)/TFLiteModel.o: $(SRC_DIR)/model/TFLiteModel.cpp $(SRC_DIR)/model/TFLiteModel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/TFLiteModel.cpp -o $@

# Clean rule
clean:
	rm -rf $(BUILD_DIR)
