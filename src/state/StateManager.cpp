#include "StateManager.hpp"

StateManager::StateManager(ThreadLogger *logger)
{
    StateManager::logger = logger;
    StateManager::stateMutex = new std::mutex();
    StateManager::state = nullptr;
    StateManager::logger->log("State manager initialized...");
    StateManager::transitioned = false;
    StateManager::shutdownQueue = new std::queue<std::function<void()>>();
}

StateManager::~StateManager()
{
    delete StateManager::state;
    delete StateManager::stateMutex;
    if (StateManager::requestedState != nullptr)
    {
        delete StateManager::requestedState;
    }
    delete StateManager::shutdownQueue;
}

IState *StateManager::getState()
{
    return StateManager::state;
}

IState *StateManager::getRequestedState()
{
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    return StateManager::requestedState;
}

void StateManager::setTransitionState(bool transitioned)
{
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    StateManager::transitioned = transitioned;
}
bool StateManager::getTransitionState()
{
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    return StateManager::transitioned;
}

int StateManager::getStateCode()
{
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);

    if (StateManager::state == nullptr)
    {
        return -1;
    }
    return StateManager::state->getStateCode();
}

void StateManager::requestState(IState *state)
{
    // sets requested state
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    StateManager::requestedState = state;
}

void StateManager::transitionTo(IState *state)
{
    // frees previous state
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    if (state == nullptr)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        StateManager::logger->log(level, "cannot transition to null state!");
        return;
    }
    if (StateManager::state != nullptr)
    {
        delete StateManager::state;
    }
    StateManager::logger->log("Transitioning to %s", state->getName().c_str());
    // sets new state
    StateManager::state = state;
    StateManager::state->setLogger(StateManager::logger);

    if (StateManager::state->getName() == "StopState")
    {
        StateManager::shutdown();
    }
}

void StateManager::runStateProcess()
{
    // runs state process

    StateManager::state->runStateProcess();
}

void StateManager::pushShutdown(std::function<void()> shutdownFunction)
{
    std::lock_guard<std::mutex> guard(*StateManager::stateMutex);
    StateManager::shutdownQueue->push(shutdownFunction);
}

void StateManager::shutdown()
{
    while (!StateManager::shutdownQueue->empty())
    {
        StateManager::shutdownQueue->front()();
        StateManager::shutdownQueue->pop();
    }
}