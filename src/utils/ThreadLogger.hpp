
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <cstdarg>
#include <iomanip>

// thread safe logger with implemented queue for threads to push logs to
#ifndef THREAD_LOGGER_HPP
#define THREAD_LOGGER_HPP

class ThreadLogger {
private:
    std::queue<std::string>* logQueue;
    std::mutex * logMutex;
    void logMessage();
    std::thread logThread;
    bool stopLogger;

public:
    ThreadLogger();
    ~ThreadLogger();

    void log(const char* format, ...);

};

#endif // THREAD_LOGGER_HPP