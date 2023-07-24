#include "IOBridge.hpp"


IOBridge::IOBridge(ThreadLogger * logger, IManager * stateManager) {
    IOBridge::running = true;
    IOBridge::logger = logger;
    IOBridge::stateManager = stateManager;
    IOBridge::mutex = new std::mutex();
    IOBridge::thread = new std::thread(&IOBridge::runLoop, this);
    logger -> log("IOBridge initialized...");
}

IOBridge::~IOBridge() {
    IOBridge::stop();
    IOBridge::thread->join();
    delete IOBridge::thread;
    delete IOBridge::mutex;
    IOBridge::logger -> log("IOBridge terminated...");

}

bool IOBridge::isRunning() {
    std::lock_guard<std::mutex> lock(*IOBridge::mutex);
    return IOBridge::running;
}

void IOBridge::stop() {
    std::lock_guard<std::mutex> lock(*IOBridge::mutex);
    IOBridge::running = false;
}

void IOBridge::runLoop() {
    int i = 0;
    while (IOBridge::isRunning()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        if (i == 10){
            IOBridge::stop();
        }else if(i % 2 == 0){
            IOBridge::logger -> log("Requesting processing state");
            IOBridge::stateManager -> requestState(new ProcessingState());
        }else{
            IOBridge::logger -> log("Requesting idling state");
            IOBridge::stateManager -> requestState(new IdlingState());
        }
        i++;
    }  
}