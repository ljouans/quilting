#include <utility>

//
// Created by jouans on 24/03/19.
//

#include <random>
#include "Picture.h"


// Easier Random to use than std's
using Random = effolkronium::random_static;

// ----------------------------------------------------------------------
// 	PUBLIC FUNCTIONS
// ----------------------------------------------------------------------

Picture::Picture() = default;
Picture::Picture(int width, int height)
{
    this->_width = width;
    this->_height = height;
    this->_data = new Pxl<int>[_width * _height];
}

Picture::Picture(const std::string& filename)
{
    Image_utils iu(filename);
    _construct(iu);
}

Picture::Picture(unsigned char* rawdata, int width, int height)
{
    this->_data = new Pxl<int>[width * height];
    this->_width = width;
    this->_height = height;

    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < height; j++)
        {
            int pij = 3*(_index(i, j));
            _data[i*height + j] = Pxl<int>(rawdata[pij], rawdata[pij + 1], rawdata[pij + 2]);
        }
    }
}


// ============================================================
// 	Getters and Setters
// ============================================================


Pxl<int> Picture::get_pxl(int i, int j) const
{
    Pxl<int> p(_data[_index(i, j)]);
    return p;
}

Picture Picture::get_tile(int i, int j, int side, int fringe) const
{
    Picture pic(side + 2*fringe, side + 2*fringe);

    if(_oob(side + i + 2*fringe, side + j + 2*fringe))
        throw std::invalid_argument( "Can't read tile out of bounds" );

    for(int id = 0; id < side + 2*fringe; id++)
    {
        for(int jd = 0; jd < side + 2*fringe; jd++)
        {
            pic(id, jd) = get_pxl(i+id, j+jd);
        }
    }

    return pic;
}

int Picture::get_width() const
{
    return this->_width;
}
int Picture::get_height() const
{
    return this->_height;
}


void Picture::set_tile(Picture source, int i, int j)
{
    if(_oob(i, j))
        throw std::invalid_argument( "Can't write tile out of bounds" );

    if(_oob(source.get_width() + i, source.get_height() + j))
        std::cerr << "Warning, writing a partial cell at (" << i << ", " << j << ")" << std::endl;


    for(int id = 0; id < source.get_width(); id++)
        for(int jd = 0; jd < source.get_height(); jd++)
            if(!_oob(i+id, j+jd))
                (*this)(i+id, j+jd) = source(id, jd);
}


Picture Picture::get_random_tile(int side, int fringe) const
{
    int i = Random::get(0, _width - side - 2*fringe);
    int j = Random::get(0, _height - side - 2*fringe);

    return (*this).get_tile(i, j, side, fringe);
}

// ============================================================
// 	PNG Manip
// ============================================================

void Picture::to_raw(unsigned char* dest)
{
    for(int i = 0; i < _width; i++)
    {
        for(int j = 0; j < _height; j++)
        {
            int pij = 3*(j*_width + i);
            Pxl<unsigned char> px = (*this)(i, j).clamped().template seen_as<unsigned char>();
            dest[pij] = px.r;
            dest[pij+1] = px.g;
            dest[pij+2] = px.b;
        }
    }
}


void Picture::write(std::string& filename)
{
    auto rawdata = new unsigned char [_width * _height * 3];
    to_raw(rawdata);

    Image_utils::write(filename, rawdata, _width, _height);

    delete[] rawdata;
}


// ============================================================
// 	Operators & Destructors
// ============================================================

Pxl<int>& Picture::operator() (int row, int col)
{
    return this->_data[_index(row, col)];
}


Picture::~Picture()
{
    if(!_data)
        delete [] _data;
}

// ----------------------------------------------------------------------
// 	PRIVATE FUNCTIONS
// ----------------------------------------------------------------------


int Picture::_index(int x, int y) const
{
	// Due to heavy performance impact, I deleted the warning. If things are well coded, this should not be an issue
    //if(_oob(x, y))
    //    throw std::runtime_error("Pic subscript index out of bounds: (" + std::to_string(x) + ", " + std::to_string(y) + ") vs (" + std::to_string(_width) + ", " + std::to_string(_height) + ")");
    return _width*y + x;

}


bool Picture::_oob(int x, int y) const {
    return (x > this->_width || y > this->_height);
}

void Picture::_construct(Image_utils &iu)
{
    this->_width = iu.get_width();
    this->_height = iu.get_height();
    this->_data = new Pxl<int>[_width * _height];


    unsigned char* rawdata = iu.get_rawdata();
    for(int i = 0; i < _width; i++)
    {
        for(int j = 0; j < _height; j++)
        {
            int pij = 3*(j*_width + i);
            _data[_index(i, j)] = Pxl<int>(rawdata[pij], rawdata[pij + 1], rawdata[pij + 2]);
        }
    }
}



