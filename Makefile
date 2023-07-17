# Define the compiler
CC := g++

# Define the source and build directories
SRC_DIR := ./src
BUILD_DIR := build

# Objects and executable
OBJS := $(addprefix $(BUILD_DIR)/, main.o segfilter.o StateManager.o IdlingState.o ProcessingState.o)
TARGET := $(BUILD_DIR)/pipeline

LIB := tensorflow

LEAK := FALSE

# Conditionally add TFLiteModel.o if target is tensorflow
ifeq ($(LIB),tensorflow)
  OBJS += $(BUILD_DIR)/TFLiteModel.o
endif

# Conditionally add TensorRTModel.o if target is nvinfer
ifeq ($(LIB),nvinfer)
  OBJS += $(BUILD_DIR)/TensorRTModel.o
endif

# Compiler flags
CFLAGS := -Wall -Werror -Wpedantic 

LINKERFLAGS := -lstdc++ 

# Conditionally add leak sanitizer
ifeq ($(LEAK),TRUE)
  CFLAGS += -fsanitize=leak
endif


# Include paths
ifeq ($(LIB),nvinfer)
  CFLAGS += -I/usr/local/cuda/include 
  LINKERFLAGS += -lcudart -lcuda -L/usr/local/cuda/lib64

endif

# Library to link against (default to tensorflow)


# Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -l$(LIB) -o $(TARGET) $(LINKERFLAGS)

# Object file rules
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/filter/IFilter.hpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/segfilter.o: $(SRC_DIR)/filter/segfilter.cpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/filter/segfilter.cpp -o $@

$(BUILD_DIR)/StateManager.o: $(SRC_DIR)/state/StateManager.cpp $(SRC_DIR)/state/StateManager.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/state/StateManager.cpp -o $@

$(BUILD_DIR)/IdlingState.o: $(SRC_DIR)/state/IdlingState.cpp $(SRC_DIR)/state/IdlingState.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/state/IdlingState.cpp -o $@

$(BUILD_DIR)/ProcessingState.o: $(SRC_DIR)/state/ProcessingState.cpp $(SRC_DIR)/state/ProcessingState.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/state/ProcessingState.cpp -o $@



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
