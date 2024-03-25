
#include <string>
#include <queue>
#include <mutex>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <cstdarg>
#include <iomanip>
#include "LoggingLevel.hpp"

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
    void insertLog(LoggingLevelWrapper LoggingLevel,const char* format, va_list args);
public:
    ThreadLogger();
    ~ThreadLogger();

    void log(const char* format, ...);
    void log(LoggingLevelWrapper LoggingLevel,const char* format, ...);
    void logPerformance(const std::string& category, const std::string& data);

};

#endif // THREAD_LOGGER_HPP