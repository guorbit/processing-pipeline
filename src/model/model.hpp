#ifndef MODEL_HPP
#define MODEL_HPP

#include <tensorflow/c/c_api.h>

class Model
{
private:
    TF_Graph* graph;
    TF_Status* status;
    TF_SessionOptions* sess_opts;
    TF_Session* session;
    TF_Buffer* run_opts;

public:
    Model(/* args */);
    ~Model();
    virtual void loadModel(const char *modelPath);
    virtual void predict(unsigned char* image, int height, int width, int channels);
    static void deallocator(void* data, size_t length, void* arg);
    virtual void Delete();
};

#endif // MODEL_HPP