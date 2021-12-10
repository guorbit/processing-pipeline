#include "exportimage.hpp"


#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "../stb_files/stb_image_write.h"

using namespace std;

ExportImage::ExportImage(unsigned char *img, int w, int h, int chan, std::string path,ThreadLogger * log) : image(img), width(w), height(h), channels(chan), filepath(path),logger(log)
{ // Declare variables and check they exist
    if (img == nullptr)
    {
        //std::cout << "The image does not exist" << std::endl;
    }

    else
    {
        //std::cout << "Variables declared by constructor" << std::endl;
    }
}
ExportImage::~ExportImage()
{ // Destructor to destroy declared variable

}

void ExportImage::SaveImage(std::string fileName)
{ // Take in stbi image and save as jpeg
    //make directory with fileName using filesystem under the directory ./outputs
    //split filename by .
    //take first part of split and make directory
    fileName = fileName.substr(0, fileName.find("."));
    fileName = "outputs/" + fileName;
    std::filesystem::create_directory(fileName);

    filepath = fileName + "/" + filepath;
    this -> logger->log("Saving image to %s", filepath.c_str());

    stbi_write_jpg(filepath.c_str(), width, height, channels, image, 100);
   
}
