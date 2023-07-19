#include "ProcessingState.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"

using namespace std;


ProcessingState::ProcessingState(){   
}

ProcessingState::~ProcessingState(){

}

std::string ProcessingState::getName(){
    return "Processing state";
}

int ProcessingState::runStateProcess(){
    this->logger->log("System entered processing state...");
    // IO reading
    int width, height, channels;
    unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);
    if (image == nullptr) {
        // Process the image
        this->logger->log("Error loading image: ", stbi_failure_reason());
        this->logger->log("Skipping processing stage...");
        return 1;
    }
    this->logger->log("Loaded image with a width of %dpx, a height of %dpx and %d channels",width,height,channels);
    


    // data processing
    SegFilter segfilter("model.engine");
    segfilter.doProcessing(image, width, height, channels);
    segfilter.doDecision();
    printf("processing done!\n");
    // image compression

    // IO writing
    printf("freeing image\n");
    stbi_image_free(image);
    printf("image freed\n");
    return 0;
}

void ProcessingState::setLogger(ThreadLogger * logger){
    this->logger = logger;
    logger->log("Processing state initialized...\n");
}