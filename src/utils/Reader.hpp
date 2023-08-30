#include "ThreadLogger.hpp"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>

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
    unsigned char * read();


private:
    ThreadLogger * logger;
    std::string mountPoint;
    std::string usbDevice;
};



#endif // READER_HPP