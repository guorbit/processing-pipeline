#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"
#include "utils/ThreadLogger.hpp"

bool isRunning = true;

void mainLoop(IManager * stateManager) {
    while (isRunning) {
        stateManager -> runStateProcess();
    }
}

int main() {
    ThreadLogger * logger = new ThreadLogger();
    logger -> log("Starting system...");
    IManager * stateManager = new StateManager(logger);

    stateManager -> transitionTo(new IdlingState());
    mainLoop(stateManager);

    delete stateManager;
    delete logger;
}

