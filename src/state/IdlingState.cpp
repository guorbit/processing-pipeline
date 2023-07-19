#include "IdlingState.hpp"

IdlingState::IdlingState(){}

IdlingState::~IdlingState(){}

std::string IdlingState::getName(){
    return "Idling state";
}

int IdlingState::runStateProcess(){
    if (firstRun){
        logger->log("System entered idling state...");
        firstRun = false;
    }
    // do nothing
    usleep(1000000); //1 second idle
    return 0;
}

void IdlingState::setLogger(ThreadLogger * logger){
    this->logger = logger;
    logger->log("Idling state initialized...");
}

