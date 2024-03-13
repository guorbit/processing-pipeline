#ifndef SEG_FILTER_H
#define SEG_FILTER_H
#include <vector>
#include "IFilter.hpp"
#ifdef USE_MOCK
#include "../hal_mock/model/MockTRT.hpp"
#else
#include "../hal_jetson/model/TensorRTModel.hpp"
#endif
#include "IModel.hpp"
#include <stdio.h>
#include "../utils/ThreadLogger.hpp"

class SegFilter: public IFilter{
private:
    /* data */
    std::vector<IModel*> models;
    ThreadLogger * logger;
public:
    SegFilter(const char *modelPath,ThreadLogger * logger);
    ~SegFilter();
    virtual int * doProcessing(unsigned char* image, int width, int height, int channels, int modelIndex);
    virtual void doDecision();
    void process(InputType input) override {
        logger->log("Processing input");
    }
};

#endif // SEG_FILTER_H