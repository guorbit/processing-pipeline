#include "ThreadLogger.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <tuple>

#ifndef READER_HPP
#define READER_HPP
namespace fs = std::filesystem;


class Reader {
public:
    Reader(ThreadLogger * logger);
    ~Reader();
    int mountDrive();
    int unmountDrive();
    bool isMounted();
    int isAvailable();
    void removeLoaded();
    std::tuple<unsigned char*, int, int, int> read();


private:
    ThreadLogger * logger;
    std::string mountPoint;
    std::string subFolder;
    std::string usbDevice;
    std::string lastLoaded = "";
    
};



#endif // READER_HPP