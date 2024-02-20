#include "TensorRTModel.hpp"

class Logger : public nvinfer1::ILogger
{
    ThreadLogger *logger;

public:
    void setLogger(ThreadLogger *logger)
    {
        Logger::logger = logger;
    }
    void log(Severity severity, const char *msg) noexcept override
    {
        // suppress info-level messages
        if (severity == Severity::kINFO)
        {
            logger->log(msg);
        }
        else if (severity == Severity::kWARNING)
        {
            LoggingLevelWrapper level(LoggingLevel::WARNING);
            logger->log(level, msg);
        }
        else if (severity == Severity::kERROR)
        {
            LoggingLevelWrapper level(LoggingLevel::ERROR);
            logger->log(level, msg);
        }
        else
        {
            logger->log(msg);
        }
    }
} gLogger;

TensorRTModel::TensorRTModel(ThreadLogger *logger)
{
    logger->log("TensorRTModel initialized!\n");
    TensorRTModel::logger = logger;
}

TensorRTModel::~TensorRTModel()
{
    delete engine;
    delete runtime;
}

std::string readBufferFromFile(const std::string &path)
{
    std::ifstream file(path, std::ios::binary);
    return {std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>()};
}

std::vector<char> readEngine(const std::string &enginePath)
{
    std::ifstream engineFile(enginePath, std::ios::binary);
    return std::vector<char>((std::istreambuf_iterator<char>(engineFile)), std::istreambuf_iterator<char>());
}

void TensorRTModel::loadModel(const char *modelPath)
{
    this->logger->log("Loading TensorRT model...\n");
    if (this->runtime != nullptr)
    {
        this->logger->log("Runtime already initialized\n");
        return;
    }
    gLogger.setLogger(this->logger);
    runtime = nvinfer1::createInferRuntime(gLogger);

    std::vector<char> engineData = readEngine(modelPath);

    if (engine != nullptr)
    {
        this->logger->log("Engine already initialized\n");
        return;
    }

    engine = runtime->deserializeCudaEngine(engineData.data(), engineData.size());

    engineData.clear();

    if (engine == nullptr)
    {
        this->logger->log("Engine not initialized\n");
        return;
    }
    else
    {
        this->logger->log("Engine initialized\n");
    }
}

int *TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{
    float *float_image = new float[height * width * channels];
    for (int i = 0; i < height * width * channels; ++i) {
        float_image[i] = static_cast<float>(image[i]);
    }


    if (engine == nullptr)
    {
        this->logger->log("Engine not initialized\n");
        return nullptr;
    }
    this->logger->log("Performing TensorRT inference...\n");
    // Allocate GPU memory for the input and output buffers
    float *gpu_input;
    float *gpu_output;
    cudaMalloc((void **)&gpu_input, sizeof(float) * height * width * channels);
    cudaMalloc((void **)&gpu_output, sizeof(float) * height * width * 7);
    // Assuming your image data is in an array called 'image' and its size is 'image_size'
    cudaMemcpy(gpu_input, float_image, sizeof(float) * height * width * channels, cudaMemcpyHostToDevice);

    // Create an execution context
    nvinfer1::IExecutionContext *context = engine->createExecutionContext();

    // Set the input and output buffers for the execution context
    void *buffers[2] = {gpu_input, gpu_output};

    // Perform inference
    context->execute(1, buffers);
    cudaError_t err;

// After each CUDA API call
    err = cudaGetLastError();
    if (err != cudaSuccess) {
        this->logger->log("CUDA error: %s\n", cudaGetErrorString(err));
    }

    // Copy the output data to the CPU memory
    float *cpu_output = new float[height * width * 7];
    cudaMemcpy(cpu_output, gpu_output, sizeof(float) * height * width * 7, cudaMemcpyDeviceToHost);
    
    delete[] float_image;
    cudaFree(gpu_input);
    cudaFree(gpu_output);

    int *max_indices = new int[height * width];
    delete context;

    // Parallelized post-processing using OpenMP
    #pragma omp parallel for
    for (int i = 0; i < height * width; ++i)
    {
        float max_value = cpu_output[i * 7];
        int max_index = 0;
        for (int j = 1; j < 7; ++j)
        {
            if (cpu_output[i * 7 + j] > max_value)
            {
                max_value = cpu_output[i * 7 + j];
                max_index = j;
            }
        }
        max_indices[i] = max_index;
        // this -> logger -> log("Max index: %s" ,std::to_string(max_index).c_str() );
    }
    // Clean up
    //! TODO: This is possibly the image output from the model so it has to be returned
        // After cudaMemcpy to get output back to CPU

    delete[] cpu_output;

   
    this->logger->log("TensorRT inference done!\n");
    return max_indices;
}