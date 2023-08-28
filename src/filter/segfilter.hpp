#ifndef SEG_FILTER_H
#define SEG_FILTER_H

#include "IFilter.hpp"
#include "../model/TFLiteModel.hpp"
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
    virtual void doProcessing(unsigned char* image, int width, int height, int channels);
    virtual void doDecision();
    
};

#endif // SEG_FILTER_H