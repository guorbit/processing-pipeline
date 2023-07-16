#include "StateManager.hpp"

StateManager::StateManager() {
    StateManager::state = nullptr;
}

StateManager::~StateManager() {
    delete StateManager::state;
}

void StateManager::setState(IState * state) {
    // frees previous state
    delete StateManager::state;
    // sets new state
    StateManager::state = state;
}

void StateManager::runStateProcess() {
    // runs state process
    StateManager::state -> runStateProcess();
}
