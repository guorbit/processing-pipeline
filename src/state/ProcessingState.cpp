#include "ProcessingState.hpp"

// #define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"

using namespace std;


ProcessingState::ProcessingState(){     
    this -> progress = 0;
}

ProcessingState::~ProcessingState(){
    this -> reader -> unmountDrive();
    delete ProcessingState::segFilter;
    delete ProcessingState::reader;
    ProcessingState::logger -> log("System exiting processing state");

}

std::string ProcessingState::getName(){
    return "Processing state";
}

int ProcessingState::getStateCode(){
    return 'p';
}

int ProcessingState::runStateProcess(){
    this -> progress = 1;
    ProcessingState::logger->log("System entered processing state...");
    // IO reading
    if(!this -> reader -> isMounted()){
        this -> reader -> mountDrive();
    }

    if (this -> reader -> isMounted()){
        this -> logger -> log("USB drive mounted successfully");
    } else {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this -> logger -> log(level,"USB drive failed to mount");
        return 1;
    }

    this -> progress = 2;

    if (!this -> reader -> isAvailable()){
        LoggingLevelWrapper level(LoggingLevel::WARNING);
        this -> logger -> log(level,"No files are availabe to process");
        return 2;
    }

    std::tuple<unsigned char*, int, int, int> imageTuple = this -> reader -> read();

    this -> progress = 3;

    unsigned char* image = std::get<0>(imageTuple);
    if (image == nullptr){
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this -> logger -> log(level,"Error reading image");
        this->logger->log("Skipping processing stage...");
        return 1;
    }
    int width = std::get<1>(imageTuple);
    int height = std::get<2>(imageTuple);
    int channels = std::get<3>(imageTuple);
    this -> progress = 4;
    // int width, height, channels;
    // unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);

    //! for some reason this function prints the data incorrectly
    //! however the data itself is correct
    this->logger->log("Loaded image with a width of %d px, a height of %d px and %d channels",width,height,channels);
    
    // data processing
    
    this -> segFilter -> doProcessing(image, width, height, channels);
    this -> progress = 5;
    this -> segFilter -> doDecision();
    this -> progress = 6;
    ProcessingState::logger -> log("Processing done");
    // image compression

    // IO writing
    
    if (image != nullptr){
        stbi_image_free(image);
    }
    
    this -> progress = 7;
    return 0;
}

void ProcessingState::setLogger(ThreadLogger * logger){
    ProcessingState::logger = logger;
    ProcessingState::segFilter = new SegFilter("model.engine",this -> logger);
    ProcessingState::reader = new Reader(this -> logger);
    logger->log("Processing state initialized...\n");

}