#include exportimage.hpp

ExportImage::ExportImage(unsigned char* image,int width,int height,int channels) : image(image), width(width), height(height), channels(channels) {

}

ExportImage::~ExportImage() {
    
    }

void ExportImage::LoadImage() { //Need to take in image from pipeline

}

void ExportImage::SaveImage() { //Take in stbi image and save as jpeg or PNG 
}

