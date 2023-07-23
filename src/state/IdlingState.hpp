#include "IState.hpp"
#include "../utils/ThreadLogger.hpp"
#include <unistd.h>
#include <stdio.h>
#include "../utils/LoggingLevel.hpp"


#ifndef IDLING_STATE_HPP
#define IDLING_STATE_HPP

class IdlingState : public IState {
private:
    ThreadLogger * logger;
    bool firstRun = true;

public:
    IdlingState();
    virtual ~IdlingState();
    virtual std::string getName();
    virtual int runStateProcess();
    virtual void setLogger(ThreadLogger * logger);
};

#endif // IDLING_STATE_HPP