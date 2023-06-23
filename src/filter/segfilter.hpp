#ifndef SEG_FILTER_H
#define SEG_FILTER_H

#include "IFilter.hpp"

class SegFilter: public IFilter{
public:
    SegFilter(/* args */);
    ~SegFilter();
    virtual void doProcessing();
    virtual void doDecision();
};

#endif // SEG_FILTER_H