#ifndef SEG_FILTER_H
#define SEG_FILTER_H
#include "IFilter.hpp"
#include "IModel.hpp"
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
    std::vector<IModel*> models;
    ThreadLogger * logger;
public:
    SegFilter(const std::vector<std::string>& modelPaths, ThreadLogger* logger);
    ~SegFilter();
    virtual int * doProcessing(unsigned char* image, int width, int height, int channels);
    virtual void doDecision();
};

#endif // SEG_FILTER_H