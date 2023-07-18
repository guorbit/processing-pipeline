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
    printf("Transitioning to new state %s...\n", state -> getName().c_str());
    // sets new state
    StateManager::state = state;
}

void StateManager::runStateProcess() {
    // runs state process
    StateManager::state -> runStateProcess();
}
