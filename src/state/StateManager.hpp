#include "IState.hpp"
#include "IManager.hpp"
#include <typeinfo> 
#include <stdio.h>
#include "../utils/LoggingLevel.hpp"
#include <thread>
#include <mutex>   

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

class StateManager : public IManager{
protected:
    IState * state;
    IState * requestedState = nullptr;
    std::mutex * stateMutex;
    ThreadLogger * logger;
public:
    StateManager(ThreadLogger * logger);
    virtual ~StateManager();
    virtual IState * getState();
    virtual IState * getRequestedState();
    virtual void requestState(IState * state);
    virtual void transitionTo(IState * state);
    virtual void runStateProcess();
};

#endif // SYSTEM_STATE_HPP