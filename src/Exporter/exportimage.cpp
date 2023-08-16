#include "exportimage.hpp"

ExportImage::ExportImage(unsigned char* img,int w,int h,int chan, std::string path) : image(img), width(w), height(h), channels(chan), filepath(path) { //Declare variables and check they exist
if(img==nullptr) {
    std::cout << "The image does not exist"<< std::endl;
 }

else{
    std::cout << "Variables declared by constructor" << std::endl;
}
}
ExportImage::~ExportImage() { //Destructor to destroy declared variables
    stbi_free_image(image);
    std::cout << "Memory has been freed" << std::endl;
    }


void ExportImage::SaveImage() { //Take in stbi image and save as jpeg
stbi_write_jpg(filepath.c_str(),width,height,channels,image,100);
std::cout << "Saving image to" << filepath << std::endl;
}