#ifndef EXP_IMAGE_H
#define EXP_IMAGE_H

#include <stdio.h>
#include <iostream>
#include <string>
#define STB_IMAGE_IMPLEMENTATION
#include "src/stb_image/stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "src/stb_image/stb_image_write.h" 

class ExportImage {
private:
    unsigned char* image;
    int width;
    int height;
    int channels;
    std::string filepath;
public:
    ExportImage();
    ~ExportImage();
    void FreeImage();
    void SaveImage();
};

#endif 