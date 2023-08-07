#include "ThreadLogger.hpp"
#include <iostream>

ThreadLogger::ThreadLogger()
{
    ThreadLogger::logQueue = new std::queue<std::string>();
    ThreadLogger::logMutex = new std::mutex();
    ThreadLogger::stopLogger = false;
    ThreadLogger::logThread = std::thread(&ThreadLogger::logMessage, this);
}

ThreadLogger::~ThreadLogger()
{
    std::lock_guard<std::mutex> lock(*ThreadLogger::logMutex);
    ThreadLogger::stopLogger = true;

    ThreadLogger::logThread.join();

    while (!ThreadLogger::logQueue->empty())
    {
        std::cout << ThreadLogger::logQueue->front() << std::endl;
        ThreadLogger::logQueue->pop();
    }


    delete ThreadLogger::logQueue;
    delete ThreadLogger::logMutex;
    std::cout << "Logger thread terminated" << std::endl;
}

void ThreadLogger::log(LoggingLevelWrapper loggingLevel,const char *format, ...)
{

    va_list args;
    va_start(args, format);

    // Determine the length of the formatted string
    int length = vsnprintf(NULL, 0, format, args);
    va_end(args); // end variable argument list

    // Allocate a buffer to hold the formatted string
    char *buffer = new char[length + 1];
    
    va_start(args, format); // start variable argument list again
    // Format the string into the buffer
    vsnprintf(buffer, length + 1, format, args);
    va_end(args); // end variable argument list
    // Get the current date and time
    time_t currentTime = time(nullptr);
    struct tm *timeinfo = localtime(&currentTime);
    
    char dateTimeString[20];
    
    strftime(dateTimeString, sizeof(dateTimeString), "%Y-%m-%d %H:%M:%S", timeinfo);

    // Create a new buffer with the date, time, and log message
    int newLength = length + 20 + 6 + loggingLevel.toString().length(); // length of dateTimeString + space + null terminator
    
    char *newBuffer = new char[newLength];
    
    std::lock_guard<std::mutex> guard(*logMutex);
    snprintf(newBuffer, newLength, "[%s] %s: %s", dateTimeString,loggingLevel.toString().c_str(), buffer);
    
    if (buffer != nullptr){
        delete[] buffer;
    }
    
    // Lock the mutex before accessing the logger object
    
    // Push the formatted string onto the log queue
    logQueue->push(newBuffer);

    if (newBuffer != nullptr){
        delete[] newBuffer;
    }
    // Unlock the mutex after accessing the logger object
    va_end(args);
}

void ThreadLogger::log(const char *format, ...){
    va_list args;
    va_start(args, format);
    LoggingLevelWrapper defaultLoggingLevel(LoggingLevel::INFO);
    ThreadLogger::log(defaultLoggingLevel, format, args);
    va_end(args);
}

void ThreadLogger::logMessage()
{
    while (!ThreadLogger::stopLogger)
    {
        if (!ThreadLogger::logQueue->empty())
        {
            std::lock_guard<std::mutex> guard(*logMutex);
            std::cout << ThreadLogger::logQueue->front() << std::endl;
            ThreadLogger::logQueue->pop();
        }
        usleep(1000);
    }
}

