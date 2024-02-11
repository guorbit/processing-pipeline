#include "state/StateManager.hpp"
#include "state/IdlingState.hpp"
#include "state/ProcessingState.hpp"
#include "utils/ThreadLogger.hpp"
#include "bridge/IOBridge.hpp"
#include <opencv2/opencv.hpp>
#include <filesystem>
#include <string>
#include <mutex>

#ifndef PIPELINE_HPP
#define PIPELINE_HPP

class Pipeline {
public:
    Pipeline();
    virtual ~Pipeline();
    virtual void run();
    cv::Mat readImage(const std::string &imagePath); // Added method to read an image
    void saveImage(const std::string &outputPath, const cv::Mat &image);

protected:
    virtual bool isRunning();
    virtual void stop();
    virtual int initDrive();

private:
    bool running;
    std::mutex * pipelineMutex;
    ThreadLogger * logger;
    StateManager * stateManager;
    IOBridge * ioBridge;
};

#endif //PIPELINE_HPP