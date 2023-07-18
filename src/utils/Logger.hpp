
#include <string>
#include <queue>
#include <mutex>
#include <thread>


// thread safe logger with implemented queue for threads to push logs to
#ifndef LOGGER_HPP
#define LOGGER_HPP

class Logger {
private:
    std::queue<std::string>* logQueue;
    std::mutex * logMutex;
    void logMessage();
    std::thread logThread;
    bool stopLogger;

public:
    Logger();
    ~Logger();
    void log(std::string message);
};

#endif // LOGGER_HPP