#include "segfilter.hpp"




void SegFilter::doProcessing(unsigned char* image, int width, int height, int channels) {
    SegFilter::model -> predict(image, width, height, channels);   
}

void SegFilter::doDecision() {
    LoggingLevelWrapper level(LoggingLevel::WARNING);
    SegFilter::logger -> log(level,"No decision maker is implemented yet. Filter automatically passed.");
}

SegFilter::SegFilter(const char *modelPath,ThreadLogger * logger) {
    
    IModel * model = new TensorRTModel();
    model -> loadModel(modelPath);
    SegFilter::model = model;

    SegFilter::logger = logger;
    logger -> log("Segmentation filter mounted");
}

SegFilter::~SegFilter() {
    
    delete SegFilter::model;
    SegFilter::logger -> log("Segmentation filter demounted");
}


