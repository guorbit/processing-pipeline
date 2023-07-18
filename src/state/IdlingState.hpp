#include "IState.hpp"
#include <unistd.h>
#include <stdio.h>

#ifndef IDLING_STATE_HPP
#define IDLING_STATE_HPP

class IdlingState : public IState {
private:
    bool firstRun = true;
public:
    IdlingState();
    virtual ~IdlingState();
    virtual std::string getName();
    virtual int runStateProcess();
};

#endif // IDLING_STATE_HPP