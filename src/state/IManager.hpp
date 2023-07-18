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
    virtual void transitionTo(IState * state) = 0;
    virtual void runStateProcess() = 0;
    virtual void setLogger(ThreadLogger * logger) = 0;    
};

#endif // I_MANAGER_HPP