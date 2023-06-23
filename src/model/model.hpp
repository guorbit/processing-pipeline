#ifndef MODEL_HPP
#define MODEL_HPP

#include <tensorflow/c/c_api.h>

class model
{
private:
    TF_Graph* graph;
    TF_Status* status;
    TF_SessionOptions* sess_opts;
    TF_Session* session;
    TF_Buffer* run_opts;
    
public:
    model(/* args */);
    ~model();
    virtual void loadModel(const char *modelPath);
    virtual void predict();
};

model::model(/* args */)
{
}

model::~model()
{
}
void model::loadModel(const char *modelPath){
    // load model
}

void model::predict(){
    // predict
}

#endif // MODEL_HPP