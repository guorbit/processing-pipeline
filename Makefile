# Define the compiler
CC := g++

# Define the source and build directories
SRC_DIR := ./src
BUILD_DIR := build

DEBUG := FALSE

# Objects and executable
OBJS := $(addprefix $(BUILD_DIR)/, main.o pipeline.o StateManager.o IdlingState.o ProcessingState.o segfilter.o IOBridge.o UART.o Reader.o exportimage.o ThreadLogger.o LoggingLevel.o )
TARGET := $(BUILD_DIR)/pipeline

LIB := tensorflow

LEAK := FALSE

DEV := TRUE

# Conditionally add TFLiteModel.o if target is tensorflow

ifeq ($(LIB),tensorflow)
  OBJS += $(BUILD_DIR)/TFLiteModel.o
endif

CFLAGS := -Wall -Werror -Wpedantic -std=c++17 -fopenmp
LINKERFLAGS := -lstdc++ -lpthread 

# Conditionally add leak sanitizer
ifeq ($(LEAK),TRUE)
  CFLAGS += -fsanitize=leak
endif

# Conditionally add debug flags
ifeq ($(DEBUG),TRUE)
  CFLAGS += -g 
endif

ifeq ($(DEV),TRUE)
  CFLAGS += -DUSE_MOCK
else ifeq ($(DEV),FALSE)
  LINKERFLAGS += -l$(LIB)

endif

ifeq ($(LIB),nvinfer)
  ifeq ($(DEV),TRUE)
	OBJS += $(BUILD_DIR)/MockTRT.o
  else
    OBJS += $(BUILD_DIR)/TensorRTModel.o
	CFLAGS += -I/usr/local/cuda/include 
    LINKERFLAGS += -lcudart -lcuda -L/usr/local/cuda/lib64
  endif
endif

HAL_FOLDER := hal_mock
# Include paths

# Executable
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(LINKERFLAGS)

# Object file rules
$(BUILD_DIR)/main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/filter/IFilter.hpp $(SRC_DIR)/filter/segfilter.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.cpp -o $@

$(BUILD_DIR)/pipeline.o: $(SRC_DIR)/pipeline.cpp $(SRC_DIR)/pipeline.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/pipeline.cpp -o $@

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

$(BUILD_DIR)/IOBridge.o: $(SRC_DIR)/bridge/IOBridge.cpp $(SRC_DIR)/bridge/IOBridge.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/bridge/IOBridge.cpp -o $@

$(BUILD_DIR)/UART.o: $(SRC_DIR)/$(HAL_FOLDER)/bridge/UART.cpp $(SRC_DIR)/$(HAL_FOLDER)/bridge/UART.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/$(HAL_FOLDER)/bridge/UART.cpp -o $@

$(BUILD_DIR)/exportimage.o: $(SRC_DIR)/Exporter/exportimage.cpp $(SRC_DIR)/Exporter/exportimage.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/Exporter/exportimage.cpp -o $@

$(BUILD_DIR)/ThreadLogger.o: $(SRC_DIR)/utils/ThreadLogger.cpp $(SRC_DIR)/utils/ThreadLogger.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/utils/ThreadLogger.cpp -o $@

$(BUILD_DIR)/LoggingLevel.o: $(SRC_DIR)/utils/LoggingLevel.cpp $(SRC_DIR)/utils/LoggingLevel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/utils/LoggingLevel.cpp -o $@

$(BUILD_DIR)/Reader.o: $(SRC_DIR)/utils/Reader.cpp $(SRC_DIR)/utils/Reader.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/utils/Reader.cpp -o $@


$(BUILD_DIR)/TFLiteModel.o: $(SRC_DIR)/model/TFLiteModel.cpp $(SRC_DIR)/model/TFLiteModel.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/model/TFLiteModel.cpp -o $@


$(BUILD_DIR)/MockTRT.o: $(SRC_DIR)/hal_mock/model/MockTRT.cpp $(SRC_DIR)/hal_mock/model/MockTRT.hpp
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $(SRC_DIR)/hal_mock/model/MockTRT.cpp -o $@




$(BUILD_DIR)/TensorRTModel.o: $(SRC_DIR)/model/TensorRTModel.cpp $(SRC_DIR)/model/TensorRTModel.hpp
		@mkdir -p $(@D)
		$(CC) $(CFLAGS) -c $(SRC_DIR)/model/TensorRTModel.cpp -o $@


# Clean rule
clean:
	rm -rf $(BUILD_DIR)
