#include "pipeline.hpp"

Pipeline::Pipeline() {
    this->pipelineMutex = new std::mutex();
    this->running = true;
    this->logger = new ThreadLogger();
    logger->log("Starting system pipeline...");
    this->stateManager = new StateManager(logger);
    this->stateManager->transitionTo(new IdlingState());
    this->ioBridge = new IOBridge(logger, stateManager);
    stateManager->pushShutdown([this]() { this->stop(); });
}

Pipeline::~Pipeline() {
    delete this->stateManager;
    delete this->ioBridge;
    delete this->logger;
    delete this->pipelineMutex;
}

bool Pipeline::isRunning() {
    std::lock_guard<std::mutex> lock(*this->pipelineMutex);
    return this->running;
}

void Pipeline::stop() {
    this->logger->log("Stopping system pipeline...");
    std::lock_guard<std::mutex> lock(*this->pipelineMutex);
    this->running = false;
}

int Pipeline::initDrive() {
    std::string mountPoint = "/media/usbdrive";
    // Unmount the USB drive
    std::string unmountCmd = "sudo umount " + mountPoint;
    int unmountResult = system(unmountCmd.c_str());

    if (unmountResult == 0) {
        logger->log("USB drive unmounted successfully from %s", mountPoint.c_str());
    } else {
        logger->log("Failed to unmount USB drive.");
    }

    return unmountResult;
}

void Pipeline::run() {
    initDrive();
    while (this->isRunning()) {
        this->stateManager->runStateProcess();
        IState* reqState = this->stateManager->getRequestedState();
        if (reqState != nullptr) {
            this->stateManager->transitionTo(reqState);
            this->stateManager->requestState(nullptr);
            this->stateManager->setTransitionState(true);
        }
    }
}

cv::Mat Pipeline::readImage(const std::string &imagePath) {
    cv::Mat image = cv::imread(imagePath, cv::IMREAD_COLOR);
    if (image.empty()) {
        logger->log("Error: Image file could not be read.");
        return cv::Mat();
    }
    logger->log("Image read successfully.");
    return image;
}

void Pipeline::saveImage(const std::string &outputPath, const cv::Mat &image) {
    if (cv::imwrite(outputPath, image)) {
        logger->log("Image saved successfully to %s", outputPath.c_str());
    } else {
        logger->log("Error: Failed to save the image.");
    }
}
