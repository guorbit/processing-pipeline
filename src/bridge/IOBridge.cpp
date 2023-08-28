#include "IOBridge.hpp"

IOBridge::IOBridge(ThreadLogger *logger, IManager *stateManager)
{
    IOBridge::running = true;
    IOBridge::logger = logger;
    IOBridge::stateManager = stateManager;
    stateManager->pushShutdown([this]()
                               { this->stop(); });
    IOBridge::mutex = new std::mutex();
    IOBridge::thread = new std::thread(&IOBridge::runLoop, this);

    logger->log("IOBridge initialized...");
}

IOBridge::~IOBridge()
{
    IOBridge::stop();
    IOBridge::thread->join();
    delete IOBridge::thread;
    delete IOBridge::mutex;

    IOBridge::logger->log("IOBridge terminated...");
}

bool IOBridge::isRunning()
{
    std::lock_guard<std::mutex> lock(*IOBridge::mutex);
    return IOBridge::running;
}

void IOBridge::stop()
{
    this->logger->log("Stopping IOBridge...");
    std::lock_guard<std::mutex> lock(*IOBridge::mutex);
    IOBridge::running = false;
}

void IOBridge::runLoop()
{
    // int i = 0;
    IOBridge::uart = new UartIO(IOBridge::logger);

    while (IOBridge::isRunning())
    {
        int state = uart->readUart();

        
        
        if (state == 'i')
        {
            IOBridge::logger->log("Requesting idling state");
            IOBridge::stateManager->requestState(new IdlingState());
        }
        else if (state == 's')
        {
            IOBridge::logger->log("Requesting system shutdown");
            IOBridge::stateManager->requestState(new StopState());
        }
        else if (state == 'p')
        {
            IOBridge::logger->log("Requesting processing state");
            IOBridge::stateManager->requestState(new ProcessingState());
        }
        bool transitioned = IOBridge::stateManager->getTransitionState(); 
        if (transitioned)
        {
            IOBridge::stateManager->setTransitionState(false);
            unsigned char msg[3];
            msg[0] = IOBridge::stateManager->getStateCode();
            msg[1] = 't';
            msg[2] = '\0';

            uart ->sendUart(msg);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    delete IOBridge::uart;
}