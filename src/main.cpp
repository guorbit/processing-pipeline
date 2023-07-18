#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"

bool isRunning = true;

void mainLoop(IManager * stateManager) {
    while (isRunning) {
        stateManager -> runStateProcess();
    }
}

int main() {
    IManager * stateManager = new StateManager();
    stateManager -> transitionTo(new IdlingState());
    mainLoop(stateManager);

    delete stateManager;
}

