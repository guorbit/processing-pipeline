#ifndef MODEL_HPP
#define MODEL_HPP

#include "IModel.hpp"
#include <tensorflow/c/c_api.h>
#include <fstream>
#include <iostream>
#include "../utils/ThreadLogger.hpp"
#include "../utils/LoggingLevel.hpp"

class TFLiteModel : public IModel{
private:
    TF_Graph* graph;
    TF_Status* status;
    TF_SessionOptions* sess_opts;
    TF_Session* session;
    TF_Buffer* run_opts;
    ThreadLogger * logger;
    static void deallocator(void* data, size_t length, void* arg);

public:
    TFLiteModel(ThreadLogger * logger);
    virtual ~TFLiteModel();
    virtual void loadModel(const char *modelPath);
    virtual int * predict(unsigned char* image, int height, int width, int channels);
};

#endif // MODEL_HPP