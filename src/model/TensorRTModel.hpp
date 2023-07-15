#ifndef MODEL_HPP
#define MODEL_HPP

#include "IModel.hpp"
#include <NvInfer.h>
#include <NvInferRuntimeCommon.h>
#include <cuda_runtime_api.h>
#include <fstream>
#include <iostream>
#include <sstream>


class TensorRTModel : public IModel{
public:
    TensorRTModel();
    virtual ~TensorRTModel();
    virtual void loadModel(const char *modelPath);
    virtual void predict(unsigned char* image, int height, int width, int channels);
};

#endif // MODEL_HPP