#include "ProcessingState.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"

using namespace std;


ProcessingState::ProcessingState(){   
}

ProcessingState::~ProcessingState(){
    ProcessingState::logger -> log("System exiting processing state");
}

std::string ProcessingState::getName(){
    return "Processing state";
}

int ProcessingState::runStateProcess(){
    ProcessingState::logger->log("System entered processing state...");
    // IO reading
    int width, height, channels;
    unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);
    if (image == nullptr) {
        // Process the image
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this->logger->log(level,"Error loading image: ", stbi_failure_reason());
        this->logger->log("Skipping processing stage...");
        return 1;
    }
    //! for some reason this function prints the data incorrectly
    //! however the data itself is correct
    this->logger->log("Loaded image with a width of %d px, a height of %d px and %d channels",width,height,channels);
    


    // data processing
    SegFilter segfilter("model.engine",ProcessingState::logger);
    segfilter.doProcessing(image, width, height, channels);
    segfilter.doDecision();
    ProcessingState::logger -> log("Processing done");
    // image compression

    // IO writing
    
    if (image != nullptr){
        stbi_image_free(image);
    }
    
    
    return 0;
}

void ProcessingState::setLogger(ThreadLogger * logger){
    ProcessingState::logger = logger;
    logger->log("Processing state initialized...\n");
}