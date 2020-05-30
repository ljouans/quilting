//
// Created by Loïc Jouans on 24/03/19.
//
#ifndef QUILTING_IMAGE_UTILS_H
#define QUILTING_IMAGE_UTILS_H


#include <string>

// This class is simply a wrapper to STBI_IMAGE, so that the entire code is not crammed into main.cpp
// (And so that multiple objects, defined into different files, may still write images)
class Image_utils {

public:
    Image_utils();

    explicit Image_utils(const std::string& filename);
    static void write(const std::string& filename, unsigned char* rawdata, int width, int height);

    int get_width() const;
    int get_height() const;
    unsigned char *get_rawdata() const;

    ~Image_utils();

private:
    int _width{}, _height{};
    unsigned char* _rawdata;
};


#endif //QUILTING_IMAGE_UTILS_H
