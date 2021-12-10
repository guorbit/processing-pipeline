#include "Reader.hpp"

#define STB_IMAGE_IMPLEMENTATION

#include "../stb_files/stb_image.h"
using namespace std;
Reader::Reader(ThreadLogger * logger){
    Reader::logger = logger;
    Reader::usbDevice = "/dev/sda1";
    Reader::mountPoint = "/media/usbdrive";
    Reader::subFolder = "/DCIM/Photo";

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
    const auto existingDir = this -> mountPoint + this -> subFolder; // Should exist in file system.
    int counter = 0;
    this -> logger -> log("Checking for files to read from %s", existingDir.c_str());
    for (const auto& entry : std::filesystem::directory_iterator(existingDir)) {
        std::string path = entry.path().string();
        counter++;
    }
    if (counter == 0){
        Reader::logger->log("No files to read");
        return 0;
    }

    return counter;
}

std::tuple<unsigned char*, int, int, int,std::string> Reader::read(){
    const auto existingDir = this -> mountPoint+ this -> subFolder; // Should exist in file system.
    Reader::logger -> log("%s", existingDir.c_str());
    if (!fs::exists(existingDir) || !fs::is_directory(existingDir)) {
        Reader::logger->log("Read target is not mounted");
        return std::make_tuple(nullptr, 0, 0, 0, "");
    }

    Reader::logger->log("Read target is mounted for reading");
    int counter = 0;
    // read the first file
    int width, height, channels;
    //log number of files in directory
    Reader::logger->log("Found %d files in directory", Reader::isAvailable());
    for (const auto& entry : std::filesystem::directory_iterator(existingDir)) {
        std::string path = entry.path().string();
        //get file name 
        std::string fileName = entry.path().filename().string();
        unsigned char * image = stbi_load(path.c_str(), &width, &height, &channels, 0);
        if (image == nullptr) {
            // Process the image
            LoggingLevelWrapper level(LoggingLevel::ERROR);
            this->logger->log(level,"Error loading image: ", stbi_failure_reason());
            this->logger->log("Skipping processing stage...");
            return std::make_tuple(nullptr, 0, 0, 0, "");
        }
        this -> lastLoaded = path;
        this -> logger -> log("Loaded image with a width of %d px, a height of %d px and %d channels",width,height,channels);
        return std::make_tuple(image, width, height, channels, fileName);
    }

    if (counter == 0){
        Reader::logger->log("No files to read");
        return std::make_tuple(nullptr, 0, 0, 0, "");
    }

    return std::make_tuple(nullptr, 0, 0, 0, "");
}

void Reader::removeLoaded(){
    if (this -> lastLoaded != ""){
        if (fs::remove(this -> lastLoaded)) {
            std::cout << "File deleted successfully.\n";
        } else {
            std::cout << "Error deleting the file.\n";
        }
    }
    this -> lastLoaded = "";
}



// int main(){
//     ThreadLogger * logger = new ThreadLogger();
//     Reader * reader = new Reader(logger);
//     if (reader->isMounted()){
//         reader->unmountDrive();
//     }
//     reader->mountDrive();
//     reader->isAvailable();
//     auto image = reader->read();
//     if (image != nullptr){
//         stbi_image_free(image);
//     }
//     reader->unmountDrive();
//     delete reader;
//     delete logger;
//     return 0;
// }
