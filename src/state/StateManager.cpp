#include "StateManager.hpp"

StateManager::StateManager(ThreadLogger * logger) {
    StateManager::logger = logger;
    StateManager::stateMutex = new std::mutex();
    StateManager::state = nullptr;
    StateManager::logger -> log("State manager initialized...");
}

StateManager::~StateManager() {
    delete StateManager::state;
    delete StateManager::stateMutex;
    if (StateManager::requestedState != nullptr){
        delete StateManager::requestedState;
    }
}

IState * StateManager::getState() {
    return StateManager::state;
}

IState * StateManager::getRequestedState() {
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    return StateManager::requestedState;
}

void StateManager::requestState(IState * state) {
    // sets requested state

    
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    StateManager::requestedState = state;
}

void StateManager::transitionTo(IState * state) {
    // frees previous state
    if (state == nullptr){
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        StateManager::logger -> log(level,"cannot transition to null state!");
        return;
    }
    if (StateManager::state != nullptr){
        delete StateManager::state;
    }
    StateManager::logger -> log("Transitioning to %s" , state -> getName().c_str());

    // sets new state
    StateManager::state = state;
    StateManager::state -> setLogger(StateManager::logger);
}

void StateManager::runStateProcess() {
    // runs state process
    StateManager::state -> runStateProcess();
}
