//
// Created by jouans on 24/03/19.
//

#include <iostream>
#include "Image_utils.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"



Image_utils::Image_utils()
{
    _rawdata = nullptr;
}

int Image_utils::get_width() const {
    return _width;
}

int Image_utils::get_height() const {
    return _height;
}

unsigned char *Image_utils::get_rawdata() const {
    return _rawdata;
}

Image_utils::Image_utils(const std::string& filename)
{
    int C;
    _rawdata = stbi_load(filename.c_str(), &_width, &_height, &C, 3);
    if (!_rawdata)
    {
        std::cout<<"loading failed"<<std::endl;
        exit(1);
    }
}

Image_utils::~Image_utils()
{
    stbi_image_free(_rawdata);
}

void Image_utils::write(const std::string& filename, unsigned char *rawdata, int width, int height)
{
    stbi_write_png(filename.c_str(), width, height, 3, rawdata, 0);
}
