//
// Created by Jouans on 21/03/19.
//

#ifndef PROJECT_CANVAS_H
#define PROJECT_CANVAS_H


#include "Picture.h"
#include "Tile.h"
#include <memory>

#define NB_CHOICES 30

class Canvas
{
public:
    // WARNIING: _tileSize is the size of the PRINTED tile. Stored tile adds a fringe of len _tileFringe all around it
    typedef std::pair<int, int> Point;

    // ============================================================
    // 	Constructors
    // ============================================================
    Canvas();
    Canvas(int nTileWidth, int nTileHeight, int tileSize, int fringe = 0);

    // ============================================================
    // 	Tile Scissor & Filling
    // ============================================================
    // Not used anymore. Was present to test the algorithms
    void fill_with_random_tiles(Picture const &origin);
    // This one select the best random tile out of NB_CHOICES
    void fill_with_best_random_tiles(Picture const& orig);
    void scissor();

    // Does the two previous steps in one go, avoiding to store all the fringes before cutting through
    void fill_and_cut(const Picture& orig);

    // ============================================================
    // 	PNG manipulation
    // ============================================================
    // Write the png file
    void write(const std::string& filename);

    // ============================================================
    // 	Destructor & operators
    // ============================================================
    // Access to the (i, j) tile without having to mess with the 2D->1D conversion
    Tile& operator() (int row, int col);
    ~Canvas();

private:

    // ============================================================
    // 	Vars
    // ============================================================
    int _nTileWidth{}, _nTileHeight{}, _tileSize;
    int _tileFringe{};
    Tile* _data;

    // ============================================================
    // 	Misc & Index
    // ============================================================
    // Test if (i, j) is out of bounds or not. 
    bool _oob(int i, int j) const;
    // Converts from 2D to 1D index
    int _index(int x, int y) const;

    // ============================================================
    // 	Write image
    // ============================================================
    // Concatenate the raw, 1D dimensionned char array collection to a single, big array, 
    // ready to be printed
    unsigned char* _to_raw();
    // Used in to_raw. Stores in <raw> data corresponmding to the tile (i, j) in the right place
    void _store_ij_tile(unsigned char* raw, int i, int j);


    // ============================================================
    // 	Scissor
    // ============================================================
    // Apply scissor on the vertical fringe (between (i, j) and (i, j-1)) 
    void _vertical_scissor_on_tile(int i, int j);
    // Idem, but for horizontal fringe
    void _horizontal_scissor_on_tile(int i, int j);

    double _pxl_distance_in_vertical_fringe(int i, int j, Point p1);
    double _pxl_distance_in_horinzontal_fringe(int i, int j, Point p1);


    // After scissoring, selects and modifies in place the tiles to update the correct pixels
    void _vertical_render_tile(int i, int j, const std::vector<int> &map, int endPt);
    void _horizontal_render_tile(int i, int j, const std::vector<int> &map, int endPt);

    // Used in the above two functions
    void _copy_line(int i, int j, Point p);
    void _copy_column(int i, int j, Point p);


};


#endif //PROJECT_CANVAS_H
