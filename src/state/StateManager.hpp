#include "IState.hpp"
#include "IManager.hpp"

#ifndef STATE_MANAGER_HPP
#define STATE_MANAGER_HPP

class StateManager : public IManager{
protected:
    IState * state;
public:
    StateManager();
    virtual ~StateManager();
    virtual void setState(IState * state);
    virtual void runStateProcess();
};

#endif // SYSTEM_STATE_HPP