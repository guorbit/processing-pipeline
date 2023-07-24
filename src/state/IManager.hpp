//
// IManager.hpp
//
#include "IState.hpp"
#include "../utils/ThreadLogger.hpp"

#ifndef I_MANAGER_HPP
#define I_MANAGER_HPP

class IManager
{
public:
    virtual ~IManager() {}
    virtual IState * getState() = 0;
    virtual IState * getRequestedState() = 0;
    virtual void requestState(IState * state) = 0;
    virtual void transitionTo(IState * state) = 0;
    virtual void runStateProcess() = 0; 
};

#endif // I_MANAGER_HPP