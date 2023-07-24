#include "IState.hpp"

#ifndef STOPSTATE_HPP
#define STOPSTATE_HPP

class StopState : public IState {
    public:
        StopState(){};
        ~StopState(){};
        std::string getName(){return "StopState";};
        int runStateProcess(){return 0;};
        void setLogger(ThreadLogger * logger){};

};

#endif //STOPSTATE_HPP