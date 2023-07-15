#include "segfilter.hpp"
#include <stdio.h>
#include "../model/model.hpp"

void SegFilter::doProcessing(unsigned char* image, int width, int height, int channels) {
    SegFilter::model -> predict(image, width, height, channels);   
}

void SegFilter::doDecision() {
    printf("SegFilter::doDecision()\n");
}

SegFilter::SegFilter(const char *modelPath) {
    
    Model * model = new Model();
    model -> loadModel(modelPath);
    SegFilter::model = model;
    printf("SegFilter initialized!\n");
}

SegFilter::~SegFilter() {
    
    delete SegFilter::model;
    printf("SegFilter destroyed!\n");
}


