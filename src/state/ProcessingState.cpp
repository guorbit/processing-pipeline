#include "ProcessingState.hpp"

// #define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb_files/stb_image_resize.h"

#include "../stb_files/stb_image.h"

using namespace std;

unsigned char* cropImageToSquare(unsigned char* imgData, int width, int height, int channels) {
    int newDim = std::min(width, height);
    unsigned char* newImgData = new unsigned char[newDim * newDim * channels];
    
    int xOffset = (width - newDim) / 2;
    int yOffset = (height - newDim) / 2;

    for (int y = 0; y < newDim; ++y) {
        for (int x = 0; x < newDim; ++x) {
            for (int c = 0; c < channels; ++c) {
                int oldIndex = ((y + yOffset) * width + (x + xOffset)) * channels + c;
                int newIndex = (y * newDim + x) * channels + c;
                newImgData[newIndex] = imgData[oldIndex];
            }
        }
    }

    return newImgData;
}

unsigned char* resizeImage(unsigned char* imgData, int width, int height, int channels, int newWidth, int newHeight) {
    unsigned char* resizedImgData = new unsigned char[newWidth * newHeight * channels];
    stbir_resize_uint8(imgData, width, height, 0, resizedImgData, newWidth, newHeight, 0, channels);
    return resizedImgData;
}



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
        usleep(1000000); //1 second idle
        return 1;
    }

    this -> progress = 2;

    if (!this -> reader -> isAvailable()){
        LoggingLevelWrapper level(LoggingLevel::WARNING);
        this -> logger -> log(level,"No files are availabe to process");
        usleep(1000000); //1 second idle
        return 2;
    }

    std::tuple<unsigned char*, int, int, int> imageTuple = this -> reader -> read();

    this -> progress = 3;

    unsigned char* image = std::get<0>(imageTuple);
    if (image == nullptr){
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this -> logger -> log(level,"Error reading image");
        this->logger->log("Skipping processing stage...");
        usleep(1000000); //1 second idle
        return 1;
    }
    int width = std::get<1>(imageTuple);
    int height = std::get<2>(imageTuple);
    int channels = std::get<3>(imageTuple);

    // cut image to 1:1 ratio
    
    unsigned char* croppedImage = cropImageToSquare(image, width, height, channels);
    stbi_image_free(image);

    // Resize image to 512x512x3

    int targetWidth = 512;
    int targetHeight = 512;

    unsigned char* resizedImage = resizeImage(croppedImage, width, height, channels, targetWidth, targetHeight);
    delete croppedImage;


    this -> progress = 4;
    // int width, height, channels;
    // unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);

    //! for some reason this function prints the data incorrectly
    //! however the data itself is correct
    this->logger->log("Loaded image with a width of %d px, a height of %d px and %d channels",width,height,channels);
    
    // data processing
    
    this -> segFilter -> doProcessing(resizedImage, width, height, channels);
    this -> progress = 5;
    this -> segFilter -> doDecision();
    this -> progress = 6;
    ProcessingState::logger -> log("Processing done");
    // image compression

    // IO writing
    
    if (image != nullptr){
        delete resizedImage;
    }
    this -> reader -> removeLoaded();
    this -> progress = 7;
    return 0;
}

void ProcessingState::setLogger(ThreadLogger * logger){
    ProcessingState::logger = logger;
    ProcessingState::segFilter = new SegFilter("model.engine",this -> logger);
    ProcessingState::reader = new Reader(this -> logger);
    logger->log("Processing state initialized...\n");

}
