#include "IState.hpp"
#include <stdio.h>
#include <iostream>
#include "../filter/segfilter.hpp"





#ifndef PROCESSING_STATE_HPP
#define PROCESSING_STATE_HPP

class ProcessingState : public IState {
public:
    ProcessingState();
    virtual ~ProcessingState();
    virtual std::string getName();
    virtual int runStateProcess();
};

#endif // PROCESSING_STATE_HPP