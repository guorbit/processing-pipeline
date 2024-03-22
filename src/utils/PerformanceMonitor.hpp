#include <string>
#include "ThreadLogger.hpp"

class PerformanceMonitor {
public:
    PerformanceMonitor();
    std::string getMemoryUsage();
    std::string getCPUUsage();
    void logPerformanceMetrics();

private:
    ThreadLogger* logger;
};

