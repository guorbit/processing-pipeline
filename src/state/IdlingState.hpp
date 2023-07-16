#include "IState.hpp"
#include <stdio.h>

#ifndef IDLING_STATE_HPP
#define IDLING_STATE_HPP

class IdlingState : public IState {
public:
    IdlingState();
    virtual ~IdlingState();
    virtual int runStateProcess();
};

#endif // IDLING_STATE_HPP