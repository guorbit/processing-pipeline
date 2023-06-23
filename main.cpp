
#include <stdio.h>
#include <tensorflow/c/c_api.h>
#include "segfilter.h"


int main() {
    printf("Hello from TensorFlow C library version %s\n", TF_Version());
    SegFilter segfilter;
    segfilter.doProcessing();
    segfilter.doDecision();
    return 0;
}