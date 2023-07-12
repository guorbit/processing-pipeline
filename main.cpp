
#include <stdio.h>
#include <tensorflow/c/c_api.h>
#include "src/filter/segfilter.hpp"
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


using namespace std;

int main() {
    printf("Hello from TensorFlow C library version %s\n", TF_Version());
    // IO reading
    int width, height, channels;
    unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);
    if (image == nullptr) {
        // Process the image
        printf("Error in loading the image\n");
        return 1;
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);


    // data processing
    SegFilter segfilter("model.pb");
    segfilter.doProcessing(image, width, height, channels);
    segfilter.doDecision();
    printf("processing done!\n");
    // image compression

    // IO writing
    printf("freeing image\n");
 
    printf("image freed\n");
    return 0;
}