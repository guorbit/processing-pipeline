#include "IState.hpp"
#include "IManager.hpp"
#include <typeinfo> 
#include <stdio.h>
#include "../utils/LoggingLevel.hpp"
#include <thread>
#include <mutex>   
#include <queue>
#include <functional>

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

class StateManager : public IManager{
protected:
    IState * state;
    IState * requestedState = nullptr;
    std::mutex * stateMutex;
    ThreadLogger * logger;
    std::queue<std::function<void()>> * shutdownQueue;

public:
    StateManager(ThreadLogger * logger);
    virtual ~StateManager();
    virtual IState * getState();
    virtual IState * getRequestedState();
    virtual void requestState(IState * state);
    virtual void transitionTo(IState * state);
    virtual void runStateProcess();
    virtual void pushShutdown(std::function<void()> shutdownFunction);
    virtual void shutdown();

};

#endif // SYSTEM_STATE_HPP