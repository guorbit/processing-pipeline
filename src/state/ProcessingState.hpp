#include "IState.hpp"
#include <stdio.h>
#include <iostream>
#include "../filter/segfilter.hpp"
#include "../utils/ThreadLogger.hpp"

#ifndef PROCESSING_STATE_HPP
#define PROCESSING_STATE_HPP

class ProcessingState : public IState {
private:
    ThreadLogger * logger;
public:
    ProcessingState();
    virtual ~ProcessingState();
    virtual std::string getName();
    virtual int runStateProcess();
    virtual void setLogger(ThreadLogger * logger);
};

#endif // PROCESSING_STATE_HPP