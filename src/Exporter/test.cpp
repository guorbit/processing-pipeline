#include <stdio.h>
#include <iostream>
#include <string>

#define STB_IMAGE_WRITE_IMPLEMENTATION 
#include "stbi_image/stbi_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stbi_image/stbi_image.h"
#include"exportimage.hpp"
#include <filesystem>

int test() {
int width,height,channels;
std::string location = std::filesystem::current_path().string() + "/image.jpg";
unsigned char* img = stbi_load("../1499_sat.jpg",&width, &height, &channels, 0);
if (img == nullptr) {
    printf("Image doesn't exist");
}

{
    ExportImage obj(img,width,height,channels,location);

    obj.saveimage;
}
return 0;
}