#include "segfilter.hpp"
#include <stdio.h>
#include "../model/model.hpp"

void SegFilter::doProcessing() {
    // SegFilter::model -> predict();
    printf("SegFilter::doProcessing()\n");
    
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
    
    SegFilter::model -> Delete();
    printf("SegFilter destroyed!\n");
}


