
#include "TensorRTModel.hpp"

TensorRTModel::TensorRTModel(const char *modelPath)
{
    TensorRTModel::loadModel(modelPath);
}

TensorRTModel::~TensorRTModel()
{
}

void TensorRTModel::loadModel(const char *modelPath)
{
    std::ifstream engineFile(modelPath, std::ios::binary);
    std::stringstream engineBuffer;
    engineBuffer << engineFile.rdbuf();

    //initialize cuda runtime
    nvinfer1::IRuntime* runtime = nvinfer1::createInferRuntime(gLogger);

    //deserialize the engine
    std::string engineStr = loadEngine("path_to_your_engine.plan");
    nvinfer1::ICudaEngine* engine = runtime->deserializeCudaEngine(engineStr.data(), engineStr.size());

    //create execution context
    nvinfer1::IExecutionContext* context = engine->createExecutionContext();

    
}

void TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{

}