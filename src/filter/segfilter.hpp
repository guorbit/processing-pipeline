#ifndef SEG_FILTER_H
#define SEG_FILTER_H

#include "IFilter.hpp"
#include "../model/model.hpp"

class SegFilter: public IFilter{
private:
    /* data */
    Model * model;
public:
    SegFilter(const char *modelPath);
    ~SegFilter();
    virtual void doProcessing(unsigned char* image, int width, int height, int channels);
    virtual void doDecision();
    
};

#endif // SEG_FILTER_H