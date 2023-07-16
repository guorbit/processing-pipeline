//
// IManager.hpp
//
#include "IState.hpp"

#ifndef I_MANAGER_HPP
#define I_MANAGER_HPP

class IManager
{
public:
    virtual ~IManager() {}
    virtual void setState(IState * state) = 0;
    virtual void runStateProcess() = 0;
};

#endif // I_MANAGER_HPP