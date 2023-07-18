#include "Logger.hpp"
#include <iostream>

Logger::Logger() {
    Logger::logQueue = new std::queue<std::string>();
    Logger::logMutex = new std::mutex();
    Logger::stopLogger = false;
    Logger::logThread = std::thread(&Logger::logMessage, this);
}

Logger::~Logger() {
    {
        std::lock_guard<std::mutex> lock(*Logger::logMutex);
        Logger::stopLogger = true;
    }
    Logger::logThread.join();
    delete Logger::logQueue;
    delete Logger::logMutex;
}

void Logger::log(std::string message) {
    std::lock_guard<std::mutex> guard(*logMutex);
    Logger::logQueue -> push(message);
}

void Logger::logMessage() {
    while (!Logger::stopLogger) {
        if (!Logger::logQueue -> empty()) {
            std::lock_guard<std::mutex> guard(*logMutex);
            std::cout << Logger::logQueue -> front() << std::endl;
            Logger::logQueue -> pop();
        }
    }
}
