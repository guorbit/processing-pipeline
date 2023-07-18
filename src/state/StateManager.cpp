#include "StateManager.hpp"

StateManager::StateManager() {
    StateManager::state = nullptr;
}

StateManager::~StateManager() {
    delete StateManager::state;
}

void StateManager::transitionTo(IState * state) {
    // frees previous state
    delete StateManager::state;
    StateManager::logger -> log("Transitioning to new state %s...", state -> getName().c_str());
    
    // sets new state
    StateManager::state = state;
    StateManager::state -> setLogger(StateManager::logger);
}

void StateManager::runStateProcess() {
    // runs state process
    StateManager::state -> runStateProcess();
}

void StateManager::setLogger(ThreadLogger * logger) {
    StateManager::logger = logger;
}
