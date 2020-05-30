//
// Created by Loïc Jouans on 24/03/19.
//
#ifndef QUILTING_TILE_H
#define QUILTING_TILE_H


#include "Picture.h"

class Tile: public Picture
{

public:
    Tile();
    Tile(const Picture& p, int fringe);

    // We consider the left (-1, 0), the upleft (-1, -1) and the up (0, -1) tiles relative to the current one
    // have already been chosen
    double dst(Tile *tleft, Tile *tupleft, Tile *tup);
    int get_fringe() const;

    ~Tile();
private:
    int _fringe{};
    double _dst_with_left(Tile *t);
    double _dst_with_up(Tile *t);
    double _dst_with_leftup(Tile *t);


};


#endif //QUILTING_TILE_H
