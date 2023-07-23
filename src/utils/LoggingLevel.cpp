#include "LoggingLevel.hpp"

LoggingLevelWrapper::LoggingLevelWrapper(LoggingLevel value) : value(value) {}
LoggingLevelWrapper::operator LoggingLevel() const { return value; }

std::string LoggingLevelWrapper::toString() const {
    switch (value) {
        case LoggingLevel::DEBUG: return "DEBUG";
        case LoggingLevel::INFO: return "INFO";
        case LoggingLevel::WARNING: return "WARNING";
        case LoggingLevel::ERROR: return "ERROR";
        case LoggingLevel::CRITICAL: return "CRITICAL";
        default: return "UNKNOWN";
    }
}