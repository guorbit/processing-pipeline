#include "IdlingState.hpp"

IdlingState::IdlingState(){
    printf("Idling state initialized...\n");
}

IdlingState::~IdlingState(){

}

std::string IdlingState::getName(){
    return "Idling state";
}

int IdlingState::runStateProcess(){
    if (firstRun){
        printf("System entered idling state...\n");
        firstRun = false;
    }
    // do nothing
    usleep(1000000); //1 second idle
    return 0;
}

