

#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "../stb_files/stb_image_write.h"
#include "../utils/ThreadLogger.hpp"

#ifndef EXPORTIMAGE_HPP
#define EXPORTIMAGE_HPP

class ExportImage
{
private:
    unsigned char *image;
    int width;
    int height;
    int channels;
    std::string filepath;
    ThreadLogger * logger;

public:
    ExportImage(unsigned char *img, int w, int h, int chan, std::string path,ThreadLogger * log);
    ~ExportImage();
    void SaveImage(std::string fileName);
};

#endif // EXPORTIMAGE_HPP
