#include "ProcessingState.hpp"

// #define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION
#include "../stb_files/stb_image_resize.h"

#include "../stb_files/stb_image.h"

using namespace std;

unsigned char *cropImageToSquare(unsigned char *imgData, int width, int height, int channels, ThreadLogger * logger)
{
    logger -> log("width %d height %d channel %d",width,height,channels);
    int newDim = std::min(width, height);
    logger -> log("cropping to width %d",newDim);
    unsigned char *newImgData = new unsigned char[newDim * newDim * channels];

    int xOffset = (width - newDim) / 2;
    int yOffset = (height - newDim) / 2;
    logger -> log("x_offset %d, y_offset %d",xOffset,yOffset);
    for (int y = 0; y < newDim; ++y)
    {
        for (int x = 0; x < newDim; ++x)
        {
            for (int c = 0; c < channels; ++c)
            {
                int oldIndex = ((y + yOffset) * width + (x + xOffset)) * channels + c;
                int newIndex = (y * newDim + x) * channels + c;
                newImgData[newIndex] = imgData[oldIndex];
            }
        }
    }

    return newImgData;
}

unsigned char *resizeImage(unsigned char *imgData, int width, int height, int channels, int newWidth, int newHeight)
{
    unsigned char *resizedImgData = new unsigned char[newWidth * newHeight * channels];
    stbir_resize_uint8(imgData, width, height, 0, resizedImgData, newWidth, newHeight, 0, channels);
    return resizedImgData;
}
unsigned char *convertMaskToRGB(int *arr, int size, int width, int height)
{
    std::map<int, std::array<unsigned char, 3>> LABEL_MAP;
    // Define the label map
    LABEL_MAP[5] = {0, 255, 255};
    LABEL_MAP[2] = {255, 255, 0};
    LABEL_MAP[4] = {255, 0, 255};
    LABEL_MAP[1] = {0, 255, 0};
    LABEL_MAP[3] = {0, 0, 255};
    LABEL_MAP[6] = {255, 255, 255};
    LABEL_MAP[0] = {0, 0, 0};

    // Allocate the output array
    unsigned char *output = new unsigned char [width * height * 3];

    // Loop through the input array and convert the values
    for (int i = 0; i < size; ++i)
    {
        int key = arr[i];
        if (LABEL_MAP.find(key) != LABEL_MAP.end())
        {
            output[i * 3] = LABEL_MAP[key][0];
            output[i * 3 + 1] = LABEL_MAP[key][1];
            output[i * 3 + 2] = LABEL_MAP[key][2];
        }
        else
        {
            // Handle the case where the key is not found in the map
            std::cout << "Warning: Key " << key << " not found in LABEL_MAP. Filling with zeros." << std::endl;
            output[i * 3] = 0;
            output[i * 3 + 1] = 0;
            output[i * 3 + 2] = 0;
        }
    }

    return output;
}

unsigned char *converToPixelMask(int * img, int width, int height, int channels)
{
    unsigned char* imageData = new unsigned char[width * height * channels];
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            for (int c = 0; c < channels; c++)
            {
                int pixelIndex = (y * width + x) * channels + c;
                imageData[pixelIndex] = static_cast<unsigned char>(img[pixelIndex]); // Convert float to unsigned char
            }
        }
    }
    return imageData;
}

ProcessingState::ProcessingState()
{
    this->progress = 0;
}

ProcessingState::~ProcessingState()
{
    this->reader->unmountDrive();
    delete ProcessingState::segFilter;
    delete ProcessingState::reader;
    ProcessingState::logger->log("System exiting processing state");
}

std::string ProcessingState::getName()
{
    return "Processing state";
}

int ProcessingState::getStateCode()
{
    return 'p';
}

int ProcessingState::runStateProcess() {
    this->progress = 1;
    ProcessingState::logger->log("System entered processing state...");
    // IO reading
    if (!this->reader->isMounted()) {
        this->reader->mountDrive();
    }

    if (this->reader->isMounted()) {
        this->logger->log("USB drive mounted successfully");
    } else {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this->logger->log(level, "USB drive failed to mount");
        usleep(1000000); // 1 second idle
        return 1;
    }

    this->progress = 2;

    if (!this->reader->isAvailable()) {
        LoggingLevelWrapper level(LoggingLevel::WARNING);
        this->logger->log(level, "No files are availabe to process");
        usleep(1000000); // 1 second idle
        return 2;
    }

    std::tuple<unsigned char *, int, int, int, std::string> imageTuple = this->reader->read();

    this->progress = 3;

    unsigned char *image = std::get<0>(imageTuple);
    if (image == nullptr) {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        this->logger->log(level, "Error reading image");
        this->logger->log("Skipping processing stage...");
        usleep(1000000); // 1 second idle
        return 1;
    }

    int width = std::get<1>(imageTuple);
    int height = std::get<2>(imageTuple);
    int channels = std::get<3>(imageTuple);
    std::string fileName = std::get<4>(imageTuple);
    this->logger->log("Loaded filename: %s", fileName.c_str());
    ExportImage oImage(image, width, height, channels, std::string("input.jpg"), this->logger);
    this->logger->log("saving input image");
    oImage.SaveImage(fileName);
    // cut image to 1:1 ratio
    this->logger->log("image saved");
    unsigned char *croppedImage = cropImageToSquare(image, width, height, channels, this->logger);
    this->logger->log("image cropped 1:1");
    stbi_image_free(image);

    // Resize image to 512x512x3
    int cropSize = std::min(width, height);
    int targetWidth = 512;
    int targetHeight = 512;

    unsigned char *resizedImage = resizeImage(croppedImage, cropSize, cropSize, channels, targetWidth, targetHeight);
    width = targetWidth;
    height = targetHeight;
    delete[] croppedImage;

    ExportImage oResizedImage(resizedImage, width, height, channels, std::string("resized_input.jpg"), this->logger);
    oResizedImage.SaveImage(fileName);

    this->progress = 4;
    // int width, height, channels;
    // unsigned char* image = stbi_load("1499_sat.jpg", &width, &height, &channels, 0);

    //! for some reason this function prints the data incorrectly
    //! however the data itself is correct
    this->logger->log("Loaded image with a width of %d px, a height of %d px and %d channels", width, height, channels);

    // data processing
    int *output = this->segFilter->doProcessing(resizedImage, width, height, channels, this->currentModelIndex);
    this->progress = 5;
    this->segFilter->doDecision();
    this->progress = 6;
    ProcessingState::logger->log("Processing done");
    // image compression
    // ! due to time constraints this part is not implemented
    // IO writing
    unsigned char *rgbOut = convertMaskToRGB(output, width * height, width, height);
    // unsigned char* pixelOut = converToPixelMask(rgbOut, width, height, 3);

    ExportImage oMask(rgbOut, width, height, 3, "output.jpg", this->logger);
    oMask.SaveImage(fileName);
    delete[] output;
    delete[] rgbOut;
    delete[] resizedImage;


    this->reader->removeLoaded();
    this->progress = 7;
    return 0;
}

void ProcessingState::setLogger(ThreadLogger *logger)
{
    ProcessingState::logger = logger;
    ProcessingState::segFilter = new SegFilter("model.engine", this->logger);
    ProcessingState::reader = new Reader(this->logger);
    logger->log("Processing state initialized...\n");
}

void ProcessingState::setCurrentModelIndex(int index) {
    this->currentModelIndex = index;
}
