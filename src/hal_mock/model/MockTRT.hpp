#ifndef TENSORRTMODEL_HPP
#define TENSORRTMODEL_HPP

#include "../../model/IModel.hpp"
#include "../../utils/ThreadLogger.hpp"
#include "../../utils/LoggingLevel.hpp"
class TensorRTModel : public IModel{
private:
    ThreadLogger * logger;
public:
    TensorRTModel(ThreadLogger * logger){this->logger = logger;};
    virtual ~TensorRTModel(){};
    virtual void loadModel(const char *modelPath){};
    virtual int * predict(unsigned char* image, int height, int width, int channels){return 0;};
};

#endif // TENSORRTMODEL_HPP