//
// IState.h
//

#include <string>
#include "../utils/ThreadLogger.hpp"
#ifndef I_STATE_HPP
#define I_STATE_HPP

class IState
{

public:
    virtual ~IState() {}
    virtual std::string getName() = 0;
    virtual int runStateProcess() = 0;
    virtual int getStateCode() = 0;
    virtual void setLogger(ThreadLogger * logger) = 0;
};

#endif // I_STATE_HPP
