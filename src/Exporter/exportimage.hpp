

#include <stdio.h>
#include <iostream>
#include <string>
#include "stb_image/stb_image.h"
#include "stb_image/stb_image_write.h" 

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION

#ifndef EXPORTIMAGE_HPP
#define EXPORTIMAGE_HPP




class ExportImage {
private:
    unsigned char* image;
    int width;
    int height;
    int channels;
    std::string filepath;
public:
    ExportImage(unsigned char* img,int w,int h,int chan, std::string path);
    ~ExportImage();
    void SaveImage();
};

#endif // EXPORTIMAGE_HPP
