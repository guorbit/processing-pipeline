#include "pipeline.hpp"

Pipeline::Pipeline(){
    this->pipelineMutex = new std::mutex();
    this->running = true;
    this->logger = new ThreadLogger();
    logger->log("Starting system pipeline...");
    this->stateManager = new StateManager(logger);
    this->stateManager->transitionTo(new IdlingState());
    this->ioBridge = new IOBridge(logger, stateManager);
    stateManager -> pushShutdown([this](){this -> stop();});
}

Pipeline::~Pipeline(){
    delete this->stateManager;
    delete this->ioBridge;
    delete this->logger;
    delete this->pipelineMutex;
}

bool Pipeline::isRunning(){
    std::lock_guard<std::mutex> lock(*this->pipelineMutex);
    return this->running;
}

void Pipeline::stop(){
    this -> logger -> log("Stopping system pipeline...");
    std::lock_guard<std::mutex> lock(*this->pipelineMutex);
    this->running = false;
}

void Pipeline::run(){
    while (this->isRunning()) {
        this->stateManager -> runStateProcess();
        IState * reqState = this->stateManager -> getRequestedState();
        if (reqState != nullptr) {
            this->stateManager -> transitionTo(reqState);
            this->stateManager -> requestState(nullptr);
        }
    }
}

