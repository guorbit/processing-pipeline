#include "PerformanceMonitor.hpp"

PerformanceMonitor::PerformanceMonitor(ThreadLogger* logger) : logger(logger) {}

std::string PerformanceMonitor::getMemoryUsage()
{
    std::string memoryUsage = "Memory Usage: ";
    return memoryUsage;
}

std::string PerformanceMonitor::getCPUUsage()
{
    std::string cpuUsage = "CPU Usage: ";
    return cpuUsage;
}

void PerformanceMonitor::logPerformanceMetrics() {
    logger->logPerformance("Memory", getMemoryUsage());
    logger->logPerformance("CPU", getCpuUsage());
}
