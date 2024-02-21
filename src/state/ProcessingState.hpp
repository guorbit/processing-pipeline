#include "IState.hpp"
#include <stdio.h>
#include <iostream>
#include "../filter/segfilter.hpp"
#include "../utils/ThreadLogger.hpp"
#include "../utils/LoggingLevel.hpp"
#include "../utils/Reader.hpp"
#include <tuple>
#include <omp.h>
#include "../Exporter/exportimage.hpp"
#include <map>
#include <vector>
#include "ThreadLogger.hpp"
#include "segfilter.hpp"

#ifndef PROCESSING_STATE_HPP
#define PROCESSING_STATE_HPP

class ProcessingState : public IState
{
private:
    ThreadLogger *logger;
    SegFilter *segFilter;
    Reader *reader;
    int currentModelIndex;

public:
    ProcessingState();
    virtual ~ProcessingState();
    virtual std::string getName();
    virtual int runStateProcess();
    virtual int getStateCode();
    virtual void setLogger(ThreadLogger *logger);
    void setCurrentModelIndex(int index);
};

void ProcessingState::setCurrentModelIndex(int index) {
    this->currentModelIndex = index;
}

#endif // PROCESSING_STATE_HPP