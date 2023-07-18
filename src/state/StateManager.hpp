#include "IState.hpp"
#include "IManager.hpp"
#include <typeinfo> 
#include <stdio.h>


#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

class StateManager : public IManager{
protected:
    IState * state;
public:
    StateManager();
    virtual ~StateManager();
    virtual void transitionTo(IState * state);
    virtual void runStateProcess();
};

#endif // SYSTEM_STATE_HPP