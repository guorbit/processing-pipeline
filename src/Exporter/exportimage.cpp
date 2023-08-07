#include exportimage.hpp

ExportImage::ExportImage(unsigned char* img,int w,int h,int chan, std::string path) : image(img), width(w), height(h), channels(chan), filepath(path) { //Declare variables and check they exist
if(img=nullptr) {
    std::cout << "The image does not exist"<< std::endl;
    return 1;
 }

else{
    std::cout << "Variables declared by constructor" << std::endl;
    return 0;
}

ExportImage::~ExportImage() { //Destructor to destroy declared variables
    
    }


void ExportImage::SaveImage() { //Take in stbi image and save as jpeg
stbi_write_jpg(filepath,width,height,channels,image);
std::cout << "Saving image to" << filepath << std::endl;

}

void ExportImage::FreeImage() { //Free up memory from loading in unsigned char image (Probably should add to destructor)
stbi_free_image(image);
std::cout << "Memory has been freed" << std::endl;
}