#include <thread>
#include <mutex>
#include "../utils/ThreadLogger.hpp"
#include "../state/IManager.hpp"
#include "../state/ProcessingState.hpp"
#include "../state/IdlingState.hpp"
#include "../state/StopState.hpp"
#include "../bridge/UART.hpp"

#ifndef IO_BRIDGE_HPP
#define IO_BRIDGE_HPP

class IOBridge {
public:
    IOBridge(ThreadLogger * logger, IManager * stateManager);
    ~IOBridge();
protected:
    void runLoop();
    bool isRunning();
    void stop();
private:
    bool running;
    std::thread * thread;
    std::mutex * mutex;
    ThreadLogger * logger;
    IManager * stateManager;
    UartIO * uart;
};


#endif // IO_BRIDGE_HPP