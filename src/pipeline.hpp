#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"
#include "utils/ThreadLogger.hpp"
#include "bridge/IOBridge.hpp"
#include <mutex>


#ifndef PIPELINE_HPP
#define PIPELINE_HPP

class Pipeline{
    public:
        Pipeline();
        virtual ~Pipeline();
        virtual void run();
    protected:
        virtual bool isRunning();
        virtual void stop();

    private:
        bool running;
        std::mutex * pipelineMutex;
        ThreadLogger * logger;
        StateManager * stateManager;
        IOBridge * ioBridge;
};

#endif //PIPELINE_HPP