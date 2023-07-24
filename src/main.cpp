#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"
#include "utils/ThreadLogger.hpp"
#include "bridge/IOBridge.hpp"
bool isRunning = true;

void mainLoop(IManager * stateManager) {
    while (isRunning) {
        stateManager -> runStateProcess();
        IState * reqState = stateManager -> getRequestedState();
        if (reqState != nullptr) {
            stateManager -> transitionTo(reqState);
            stateManager -> requestState(nullptr);
        }
    }
}

int main() {
    ThreadLogger * logger = new ThreadLogger();
    logger -> log("Starting system...");
    IManager * stateManager = new StateManager(logger);
    stateManager -> transitionTo(new IdlingState());
    IOBridge * ioBridge = new IOBridge(logger, stateManager);
    mainLoop(stateManager);

    delete stateManager;
    delete logger;
    delete ioBridge;
}

