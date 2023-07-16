# Define the compiler
CC := g++

# Define the source and build directories
SRC_DIR := ./src
BUILD_DIR := build

# Objects and executable
OBJS := $(addprefix $(BUILD_DIR)/, main.o segfilter.o)
TARGET := $(BUILD_DIR)/pipeline

LIB := tensorflow

# Conditionally add TFLiteModel.o if target is tensorflow
ifeq ($(LIB),tensorflow)
  OBJS += $(BUILD_DIR)/TFLiteModel.o
endif

# Conditionally add TensorRTModel.o if target is nvinfer
ifeq ($(LIB),nvinfer)
  OBJS += $(BUILD_DIR)/TensorRTModel.o
endif

# Compiler flags
CFLAGS := -Wall -Werror -Wpedantic -I/usr/local/cuda/include

# Library to link against (default to tensorflow)


# Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -l$(LIB) -o $(TARGET) -lstdc++

# Object file rules
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/filter/IFilter.hpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/segfilter.o: $(SRC_DIR)/filter/segfilter.cpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/filter/segfilter.cpp -o $@

ifeq ($(LIB),tensorflow)
$(BUILD_DIR)/TFLiteModel.o: $(SRC_DIR)/model/TFLiteModel.cpp $(SRC_DIR)/model/TFLiteModel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/TFLiteModel.cpp -o $@
endif

ifeq ($(LIB),nvinfer)
$(BUILD_DIR)/TensorRTModel.o: $(SRC_DIR)/model/TensorRTModel.cpp $(SRC_DIR)/model/TensorRTModel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/TensorRTModel.cpp -o $@
endif
# Clean rule
clean:
	rm -rf $(BUILD_DIR)
