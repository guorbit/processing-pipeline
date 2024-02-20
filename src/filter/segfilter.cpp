#include "segfilter.hpp"
#include "../Exporter/exportimage.hpp"  // Include ExportImage header

int * SegFilter::doProcessing(unsigned char* image, int width, int height, int channels) {
    return SegFilter::model -> predict(image, width, height, channels);
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

SegFilter::SegFilter(const char *modelPath, ThreadLogger * logger) {
    logger -> log("Mounting segmentation filter...");
    IModel * model = new TensorRTModel(logger);
    model -> loadModel(modelPath);
    SegFilter::model = model;

    SegFilter::logger = logger;
    logger -> log("Segmentation filter mounted");
}

SegFilter::~SegFilter() {
    delete SegFilter::model;
    SegFilter::logger -> log("Segmentation filter demounted");
}




