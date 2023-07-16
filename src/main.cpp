#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"

int main() {
    IManager * stateManager = new StateManager();

    //perform state cycling
    stateManager -> setState(new IdlingState());
    stateManager -> runStateProcess();
    stateManager -> setState(new ProcessingState());
    stateManager -> runStateProcess();
}