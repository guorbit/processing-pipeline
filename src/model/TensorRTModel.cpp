#include "TensorRTModel.hpp"

TensorRTModel::TensorRTModel()
{
    printf("TensorRTModel initialized!\n");
}

TensorRTModel::~TensorRTModel()
{
}

void TensorRTModel::loadModel(const char *modelPath)
{
    // Create a TensorRT engine
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(gLogger);
    
    // Load the engine file
    std::ifstream engineFile("your_model.engine", std::ios::binary);
    std::string engineStr((std::istreambuf_iterator<char>(engineFile)), std::istreambuf_iterator<char>());
    
    // Deserialize the engine
    nvinfer1::ICudaEngine* engine = runtime->deserializeCudaEngine(engineStr.data(), engineStr.size(), nullptr);
    
    // Create an execution context
    nvinfer1::IExecutionContext* context = engine->createExecutionContext();
    
    // TODO: Allocate input and output buffers, copy input data, run inference, and retrieve the output
    
    // Clean up
    context->destroy();
    engine->destroy();
    runtime->destroy();
    
    return 0;
    
}

void TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{

}