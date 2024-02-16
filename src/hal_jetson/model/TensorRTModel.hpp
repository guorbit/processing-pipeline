#ifndef TENSORRTMODEL_HPP
#define TENSORRTMODEL_HPP

#include "../../model/IModel.hpp"
#include <NvInfer.h>
#include <NvInferRuntimeCommon.h>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "../../utils/ThreadLogger.hpp"
#include "../../utils/LoggingLevel.hpp"

class TensorRTModel : public IModel{
private:
    nvinfer1::ICudaEngine* engine = nullptr;
    nvinfer1::IRuntime* runtime = nullptr;
    ThreadLogger * logger;
public:
    TensorRTModel(ThreadLogger * logger);
    virtual ~TensorRTModel();
    virtual void loadModel(const char *modelPath);
    virtual int * predict(unsigned char* image, int height, int width, int channels);
};


 // TENSORRTMODEL_HPP
