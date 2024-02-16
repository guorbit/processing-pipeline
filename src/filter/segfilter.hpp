#ifndef SEG_FILTER_H
#define SEG_FILTER_H

#include "IFilter.hpp"
#ifdef USE_MOCK
#include "../hal_mock/model/MockTRT.hpp"
#else
#include "../hal_jetson/model/TensorRTModel.hpp"
#endif


#include <stdio.h>
#include "../utils/ThreadLogger.hpp"

class SegFilter: public IFilter{
private:
    /* data */
    IModel * model;
    ThreadLogger * logger;
public:
    SegFilter(const char *modelPath,ThreadLogger * logger);
    ~SegFilter();
    virtual int * doProcessing(unsigned char* image, int width, int height, int channels);
    virtual void doDecision();
    
};

#endif // SEG_FILTER_H