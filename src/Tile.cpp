//
// Created by jouans on 24/03/19.
//

#include "Tile.h"

// ----------------------------------------------------------------------
// 	PUBLIC FUNCTIONS
// ----------------------------------------------------------------------
Tile::Tile() = default;


Tile::Tile(const Picture& p, int fringe):
    Picture(p), _fringe(fringe)
{
}

double Tile::dst(Tile *tleft, Tile *tleftup, Tile *tup)
{
    double dstLeft = _dst_with_left(tleft);
    double dstleftup = _dst_with_leftup(tleftup);
    double dstUp = _dst_with_up(tup);

    return dstleftup + dstLeft + dstUp;
}

int Tile::get_fringe() const
{
    return _fringe;
}

Tile::~Tile() = default;

// ----------------------------------------------------------------------
// 	PRIVATE FUNCTIONS
// ----------------------------------------------------------------------
double Tile::_dst_with_left(Tile *tLeft)
{
    if(tLeft == nullptr)
        return 0.;

    double dst = 0;

    for(int i = 0; i < 2*tLeft->get_fringe(); i++)
    {
        for(int j = 2*tLeft->get_fringe(); j < tLeft->get_height(); j++)
        {
            int shiftLeft = tLeft->get_width() - 2*tLeft->get_fringe();
            dst += (*this)(i, j).dst((*tLeft)(i + shiftLeft, j));
        }
    }

    return dst;
}

double Tile::_dst_with_up(Tile *tUp)
{
    if(tUp == nullptr)
        return 0.;

    double dst = 0;

    for(int i = 2*tUp->get_fringe(); i < tUp->get_width(); i++)
    {
        for(int j = 0; j < 2*tUp->get_fringe(); j++)
        {
            int shiftUp = tUp->get_height() - 2*tUp->get_fringe();
            dst += (*this)(i, j).dst((*tUp)(i, j + shiftUp));
        }
    }

    return dst;
}

double Tile::_dst_with_leftup(Tile *t)
{
    if(t == nullptr)
        return 0.;

    double dst = 0;

    for(int i = 0; i < 2*t->get_fringe(); i++)
    {
        for(int j = 0; j < 2*t->get_fringe(); j++)
        {
            int shiftW = t->get_width() - 2*t->get_fringe();
            int shiftH = t->get_height() - 2*t->get_fringe();
            dst += (*this)(i, j).dst((*t)(i + shiftW, j + shiftH));
        }
    }

    return dst;
}


