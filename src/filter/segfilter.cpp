#include "segfilter.hpp"




int * SegFilter::doProcessing(unsigned char* image, int width, int height, int channels) {
    return SegFilter::model -> predict(image, width, height, channels);   
}

void SegFilter::doDecision() {
    LoggingLevelWrapper level(LoggingLevel::WARNING);
    SegFilter::logger -> log(level,"No decision maker is implemented yet. Filter automatically passed.");
}

SegFilter::SegFilter(const char *modelPath,ThreadLogger * logger) {
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


