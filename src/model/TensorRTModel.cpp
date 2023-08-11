#include "TensorRTModel.hpp"

class Logger : public nvinfer1::ILogger
{
    ThreadLogger *logger;
    
    public:void setLogger(ThreadLogger *logger)
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

void TensorRTModel::predict(unsigned char *image, int height, int width, int channels)
{
    if (engine == nullptr)
    {
        this->logger->log("Engine not initialized\n");
        return;
    }
    this->logger->log("Performing TensorRT inference...\n");
    // Allocate GPU memory for the input and output buffers
    float *gpu_input;
    float *gpu_output;
    cudaMalloc((void **)&gpu_input, sizeof(float) * height * width * channels);
    cudaMalloc((void **)&gpu_output, sizeof(float) * height * width * 7);

    // Create an execution context
    nvinfer1::IExecutionContext *context = engine->createExecutionContext();

    // Set the input and output buffers for the execution context
    void *buffers[2] = {gpu_input, gpu_output};

    // Perform inference
    context->execute(1, buffers);

    // Copy the output data to the CPU memory
    float *cpu_output = new float[height * width * 7];
    cudaMemcpy(cpu_output, gpu_output, sizeof(float) * height * width * 7, cudaMemcpyDeviceToHost);

    // Clean up
    //! TODO: This is possibly the image output from the model so it has to be returned
    delete[] cpu_output;

    cudaFree(gpu_input);
    cudaFree(gpu_output);
    delete context;
    this->logger->log("TensorRT inference done!\n");
}