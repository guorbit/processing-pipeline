#ifndef TENSORRTMODEL_HPP
#define TENSORRTMODEL_HPP

#include "IModel.hpp"
#include <NvInfer.h>
#include <NvInferRuntimeCommon.h>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>

class TensorRTModel : public IModel{
private:
    nvinfer1::ICudaEngine* engine;
    nvinfer1::IRuntime* runtime;
public:
    TensorRTModel();
    virtual ~TensorRTModel();
    virtual void loadModel(const char *modelPath);
    virtual void predict(unsigned char* image, int height, int width, int channels);
};

#endif // TENSORRTMODEL_HPP
