//
// IState.h
//
#ifndef I_STATE_HPP
#define I_STATE_HPP

class IState
{

public:
    virtual ~IState() {}
    virtual int runStateProcess() = 0;
};

#endif // I_STATE_HPP
