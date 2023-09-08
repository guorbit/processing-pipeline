#include "IState.hpp"
#include <stdio.h>
#include <iostream>
#include "../filter/segfilter.hpp"
#include "../utils/ThreadLogger.hpp"
#include "../utils/LoggingLevel.hpp"
#include "../utils/Reader.hpp"
#include <tuple>
#include <omp.h>

#ifndef PROCESSING_STATE_HPP
#define PROCESSING_STATE_HPP

class ProcessingState : public IState
{
private:
    ThreadLogger *logger;
    SegFilter *segFilter;
    Reader *reader;
    int progress;

public:
    ProcessingState();
    virtual ~ProcessingState();
    virtual std::string getName();
    virtual int runStateProcess();
    virtual int getStateCode();
    virtual void setLogger(ThreadLogger *logger);
};

#endif // PROCESSING_STATE_HPP