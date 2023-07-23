#include <string>
#ifndef LOGGING_LEVEL_H
#define LOGGING_LEVEL_H


enum class LoggingLevel{
    DEBUG,
    INFO,
    WARNING,
    ERROR,
    CRITICAL
};

class LoggingLevelWrapper
{
public:
    LoggingLevelWrapper(LoggingLevel value);
    operator LoggingLevel() const;
    std::string toString() const;

private:
    LoggingLevel value;
};
#endif // LOGGING_LEVEL_H