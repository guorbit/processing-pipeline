#include "IState.hpp"

#ifndef STOPSTATE_HPP
#define STOPSTATE_HPP

class StopState : public IState {
    public:
        StopState(){};
        ~StopState(){};
        std::string getName(){return "StopState";};
        int getStateCode(){return 's';};
        int runStateProcess(){return 0;};
        void setLogger(ThreadLogger * logger){
            logger->log("stop set");
        };

};

#endif //STOPSTATE_HPP