#include <stdio.h>
#include <iostream>
#include <string>
#include <filesystem>
#include "exportimage.hpp"

int test()
{
    int width, height, channels;
    std::string location = std::filesystem::current_path().string() + "/image.jpg";
    unsigned char *img = stbi_load("../../1499_sat.jpg", &width, &height, &channels, 0);
    if (img == nullptr)
    {
        printf("Image doesn't exist");
    }

    {
        ExportImage obj(img, width, height, channels, location);

        obj.SaveImage();
    }
    return 0;
}

int main()
{
    test();
    return 0;
}
