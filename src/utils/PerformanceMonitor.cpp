#include "PerformanceMonitor.hpp"

class PerformanceMonitor {
public:
    std::string getMemoryUsage();
};

std::string PerformanceMonitor::getMemoryUsage()
{
    struct sysinfo memInfo;
    sysinfo(&memInfo);

    long long totalPhysMem = memInfo.totalram;
    totalPhysMem *= memInfo.mem_unit;

    long long physMemUsed = memInfo.totalram - memInfo.freeram;
    physMemUsed *= memInfo.mem_unit;

    std::string memoryUsage = "Memory Usage: ";
    memoryUsage += std::to_string(physMemUsed / 1024 / 1024) + " MB / ";
    memoryUsage += std::to_string(totalPhysMem / 1024 / 1024) + " MB";
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
