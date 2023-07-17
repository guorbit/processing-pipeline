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
    if (runtime != nullptr) {
        printf("Runtime already initialized\n");
        return;
    }
    runtime = nvinfer1::createInferRuntime(gLogger);

    std::vector<char> engineData = readEngine(modelPath);

    if (engine != nullptr) {
        printf("Engine already initialized\n");
        return;
    }

    engine = runtime->deserializeCudaEngine(engineData.data(), engineData.size());

    engineData.clear();
    
    if (engine == nullptr) {
        printf("Error loading engine\n");
        return;
    }
}

void TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{
    if (engine == nullptr) {
        printf("Engine not initialized\n");
        return;
    }
    printf("Running TensorRT inference on GPU\n");
        // Allocate GPU memory for the input and output buffers
    float* gpu_input;
    float* gpu_output;
    cudaMalloc((void**)&gpu_input, sizeof(float) * height * width * channels);
    cudaMalloc((void**)&gpu_output, sizeof(float) * height * width * channels);

        // Create an execution context
    nvinfer1::IExecutionContext* context = engine->createExecutionContext();

    // Set the input and output buffers for the execution context
    void* buffers[2] = { gpu_input, gpu_output };

    // Perform inference
    context->execute(1, buffers);

    // Copy the output data to the CPU memory
    float* cpu_output = new float[height * width * channels];
    cudaMemcpy(cpu_output, gpu_output, sizeof(float) * height * width * channels, cudaMemcpyDeviceToHost);

    // Clean up
    //!TODO: This is possibly the image output from the model so it has to be returned
    delete[] cpu_output;
    
    cudaFree(gpu_input);
    cudaFree(gpu_output);
    delete context;
    printf("TensorRT inference done!\n");
}