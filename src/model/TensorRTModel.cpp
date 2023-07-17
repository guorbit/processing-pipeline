#include "TensorRTModel.hpp"

class Logger : public nvinfer1::ILogger           
{
    void log(Severity severity, const char* msg) noexcept override
    {
        // suppress info-level messages
        if (severity != Severity::kINFO)
            std::cout << msg << std::endl;
    }
} gLogger;


TensorRTModel::TensorRTModel()
{
    printf("TensorRTModel initialized!\n");
}

TensorRTModel::~TensorRTModel()
{
    delete engine;
    delete runtime;
}

std::string readBufferFromFile(const std::string& path)
{
    std::ifstream file(path, std::ios::binary);
    return { std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>() };
}

std::vector<char> readEngine(const std::string& enginePath)
{
    std::ifstream engineFile(enginePath, std::ios::binary);
    return std::vector<char>((std::istreambuf_iterator<char>(engineFile)), std::istreambuf_iterator<char>());
}


void TensorRTModel::loadModel(const char *modelPath)
{
    std::cout << "TensorRT version: "
          << NV_TENSORRT_MAJOR << "." 
          << NV_TENSORRT_MINOR << "." 
          << NV_TENSORRT_PATCH << "." 
          << NV_TENSORRT_BUILD << std::endl;
        // Create a TensorRT runtime
    runtime = nvinfer1::createInferRuntime(gLogger);

    std::vector<char> engineData = readEngine(modelPath);

    engine = runtime->deserializeCudaEngine(engineData.data(), engineData.size());
}

void TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{

}