#include "segfilter.hpp"
#include "../Exporter/exportimage.hpp"  // Include ExportImage header

void SegFilter::doDecision(int *output, unsigned char *resizedImage, int width, int height, int channels, const std::string& fileName) {
    const int DECISION_THRESHOLD = 80;
    int decisionMetric = output[0];
    if (decisionMetric > DECISION_THRESHOLD) {
        ExportImage oImage(resizedImage, width, height, channels, "kept_image.jpg", this->logger);
        oImage.SaveImage(fileName);
        this->logger->log("Image kept based on model decision.");
    } else {
        this->logger->log("Image discarded based on model decision.");
    }
}

SegFilter::SegFilter(const std::vector<std::string>& modelPaths, ThreadLogger * logger) {
    this->logger = logger;
    logger->log("Mounting segmentation filters...");
    for (const auto& path : modelPaths) {
        IModel * model = new TensorRTModel(logger);
        model->loadModel(path.c_str());
        this->models.push_back(model);
    }
    logger->log("Segmentation filters mounted");
}

SegFilter::~SegFilter() {
    delete SegFilter::model;
    SegFilter::logger -> log("Segmentation filter demounted");
}

int * SegFilter::doProcessing(unsigned char* image, int width, int height, int channels, int modelIndex) {
    // Validate model index
    if (modelIndex < 0 || modelIndex >= models.size()) {
        logger->log("Invalid model index: %d", modelIndex);
        return nullptr;
    }

    logger->log("Performing processing using model at index: %d", modelIndex);
    return models[modelIndex]->predict(image, width, height, channels);
}

void SegFilter::doDecision(int *output, unsigned char *resizedImage, int width, int height, int channels, const std::string& fileName) {
    const int DECISION_THRESHOLD = 80;
    int decisionMetric = output[0];
    if (decisionMetric > DECISION_THRESHOLD) {
        ExportImage oImage(resizedImage, width, height, channels, "kept_image.jpg", this->logger);
        oImage.SaveImage(fileName);
        this->logger->log("Image kept based on model decision.");
    } else {
        this->logger->log("Image discarded based on model decision.");
    }
}




