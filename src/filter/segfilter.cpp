#include "segfilter.hpp"
#include <stdio.h>



void SegFilter::doProcessing(unsigned char* image, int width, int height, int channels) {
    SegFilter::model -> predict(image, width, height, channels);   
}

void SegFilter::doDecision() {
    printf("SegFilter::doDecision()\n");
}

SegFilter::SegFilter(const char *modelPath) {
    
    IModel * model = new TFLiteModel();
    model -> loadModel(modelPath);
    SegFilter::model = model;
    printf("SegFilter initialized!\n");
}

SegFilter::~SegFilter() {
    
    delete SegFilter::model;
    printf("SegFilter destroyed!\n");
}


