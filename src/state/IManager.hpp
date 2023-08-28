//
// IManager.hpp
//
#include "IState.hpp"
#include "../utils/ThreadLogger.hpp"
#include <functional>

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
    virtual void setTransitionState(bool transitioned) = 0;
    virtual bool getTransitionState() = 0;
    virtual int getStateCode() = 0;
    virtual void runStateProcess() = 0; 
    virtual void pushShutdown(std::function<void()> shutdownFunction) = 0;
    virtual void shutdown() = 0;
};

#endif // I_MANAGER_HPP