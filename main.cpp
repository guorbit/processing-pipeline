
#include <stdio.h>
#include <tensorflow/c/c_api.h>
#include "src/filter/segfilter.hpp"


int main() {
    printf("Hello from TensorFlow C library version %s\n", TF_Version());
    // IO reading

    // data processing
    SegFilter segfilter("model.tflite");
    segfilter.doProcessing();
    segfilter.doDecision();
    // image compression

    // IO writing
    return 0;
}