#include "ProcessingState.hpp"
#define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"

using namespace std;


ProcessingState::ProcessingState(){

}

ProcessingState::~ProcessingState(){

}

int ProcessingState::runStateProcess(){
    // IO reading
    int width, height, channels;
    unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);
    if (image == nullptr) {
        // Process the image
        printf("Error loading image: %s\n", stbi_failure_reason());
        printf("Skipping processing stage...\n");
        return 1;
    }
    printf("Loaded image with a width of %dpx, a height of %dpx and %d channels\n", width, height, channels);


    // data processing
    SegFilter segfilter("model.engine");
    segfilter.doProcessing(image, width, height, channels);
    segfilter.doDecision();
    printf("processing done!\n");
    // image compression

    // IO writing
    printf("freeing image\n");
 
    printf("image freed\n");
    return 0;
}