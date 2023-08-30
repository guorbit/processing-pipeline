#include "Reader.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"
using namespace std;
Reader::Reader(ThreadLogger * logger){
    Reader::logger = logger;
    Reader::usbDevice = "/dev/sda1";
    Reader::mountPoint = "/media/usbdrive";
}

Reader::~Reader(){
    Reader::logger->log("Reader destroyed");
}

int Reader::mountDrive(){
    // Identify the USB drive (usually /dev/sda1, /dev/sdb1, etc.)


    // Create the mount point directory
    std::string mkdirCmd = "sudo mkdir -p " + this -> mountPoint;
    system(mkdirCmd.c_str());

    // Mount the USB drive
    std::string mountCmd = "sudo mount " + this -> usbDevice + " " + this -> mountPoint;
    int result = system(mountCmd.c_str());

    if (result == 0) {
        Reader::logger->log("USB drive mounted successfully at %s", this -> mountPoint.c_str());
    } else {
        Reader::logger->log("Failed to mount USB drive.");
    }
    return result;
}

int Reader::unmountDrive(){
 
    // Unmount the USB drive
    std::string unmountCmd = "sudo umount " + this -> mountPoint;
    int unmountResult = system(unmountCmd.c_str());

    if (unmountResult == 0) {
        Reader::logger->log("USB drive unmounted successfully from %s", this -> mountPoint.c_str());
    } else {
        Reader::logger->log("Failed to unmount USB drive.");
    }

    return unmountResult;
}

bool Reader::isMounted(){
    std::ifstream mountsFile("/proc/mounts");
    std::string line;

    if (!mountsFile.is_open()) {
        std::cerr << "Failed to open /proc/mounts" << std::endl;
        return false;
    }

    while (getline(mountsFile, line)) {
        if (line.find(this -> usbDevice) != std::string::npos && line.find(this -> mountPoint) != std::string::npos) {
            this -> logger -> log("USB drive is mounted");
            return true;
        }
    }
    this -> logger -> log("USB drive is not mounted");
    return false;
}

//Maybe obsolete and not going to be needed.
//Further testing is required.
int Reader::isAvailable(){
    const auto existingDir = this -> mountPoint; // Should exist in file system.
    if (!fs::exists(existingDir) || !fs::is_directory(existingDir)) {
        Reader::logger->log("Read target is not mounted");
        return -1;
    }
    Reader::logger->log("Read target is mounted");

    int counter = 0;

    for (const auto& entry : std::filesystem::directory_iterator(existingDir)) {
        std::string path = entry.path().string();
        std::string message = "Found file: " + path;
        Reader::logger->log(message.c_str());
        counter++;
    }
    if (counter == 0){
        Reader::logger->log("No files to read");
        return -0;
    }

    return 1;
}

unsigned char * Reader::read(){
    const auto existingDir = this -> mountPoint; // Should exist in file system.
    if (!fs::exists(existingDir) || !fs::is_directory(existingDir)) {
        Reader::logger->log("Read target is not mounted");
        return nullptr;
    }

    Reader::logger->log("Read target is mounted for reading");
    int counter = 0;
    // read the first file
    int width, height, channels;
    for (const auto& entry : std::filesystem::directory_iterator(existingDir)) {
        std::string path = entry.path().string();
        std::string message = "Found file: " + path;
        Reader::logger->log(message.c_str());
        unsigned char * image = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (image == nullptr) {
            // Process the image
            LoggingLevelWrapper level(LoggingLevel::ERROR);
            this->logger->log(level,"Error loading image: ", stbi_failure_reason());
            this->logger->log("Skipping processing stage...");
            return nullptr;
        }
        this -> logger -> log("Loaded image with a width of %d px, a height of %d px and %d channels",width,height,channels);
        return image;
    }

    if (counter == 0){
        Reader::logger->log("No files to read");
        return nullptr;
    }

    return nullptr;
}

int main(){
    ThreadLogger * logger = new ThreadLogger();
    Reader * reader = new Reader(logger);
    if (reader->isMounted()){
        reader->unmountDrive();
    }
    reader->mountDrive();
    reader->isAvailable();
    auto image = reader->read();
    if (image != nullptr){
        stbi_image_free(image);
    }
    reader->unmountDrive();
    delete reader;
    delete logger;
    return 0;
}