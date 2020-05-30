#include <utility>

//
// Created by Jouans on 21/03/19.
//

#include "Canvas.h"
#include "Image_utils.h"

// ----------------------------------------------------------------------
// 		PUBLIC FUNCTIONS
// ----------------------------------------------------------------------

// ============================================================
// 	Constructors
// ============================================================
Canvas::Canvas()
{
    _tileSize = -1;
    _data = nullptr;
}

Canvas::Canvas(int nTileWidth, int nTileHeight, int tilesize, int tilefringe):
        _nTileWidth(nTileWidth), _nTileHeight(nTileHeight), _tileSize(tilesize), _tileFringe(tilefringe)
{
    _data = new Tile[nTileHeight*nTileWidth];
}

// ============================================================
// 	Tile Scissor & Filling
// ============================================================
void Canvas::fill_with_random_tiles(Picture const &origin)
{

    for(int i = 0; i < _nTileWidth; i++)
            for(int j = 0; j < _nTileHeight; j++)
                if(!_oob(i, j))
                {
                    Tile t(origin.get_random_tile(_tileSize, _tileFringe), _tileFringe);
                    (*this)(i, j) = t;
                }
}

void Canvas::fill_with_best_random_tiles(Picture const &orig)
{
    for(int i = 0; i < _nTileWidth; i++)
    {
        for (int j = 0; j < _nTileHeight; j++)
        {
            Tile *tUp = (_oob(i, j-1) ? nullptr : &(*this)(i, j-1));
            Tile *tUpLeft = (_oob(i-1, j-1) ? nullptr : &(*this)(i-1, j-1));
            Tile *tLeft = (_oob(i-1, j) ? nullptr : &(*this)(i-1, j));

            Tile tBest;
            double minDist = INFINITY;
            for(int k = 0; k < NB_CHOICES; k++)
            {
                Tile t2(orig.get_random_tile(_tileSize, _tileFringe), _tileFringe);
                double d2 = t2.dst(tLeft, tUpLeft, tUp);

                if(d2 < minDist)
                {
                    minDist = d2;
                    tBest = t2;
                }

            }
            (*this)(i, j) = tBest;
        }
    }
}

void Canvas::scissor()
{
    for (int i = 0; i < _nTileWidth; ++i)
    {
        for (int j = 0; j < _nTileHeight; ++j)
        {
            _vertical_scissor_on_tile(i, j);
            _horizontal_scissor_on_tile(i, j);
        }
    }

}

void Canvas::fill_and_cut(const Picture& orig)
{
    for (int i = 0; i < _nTileWidth; ++i)
    {
        for (int j = 0; j < _nTileHeight; ++j)
        {
            Tile *tUp = (_oob(i, j-1) ? nullptr : &(*this)(i, j-1));
            Tile *tUpLeft = (_oob(i-1, j-1) ? nullptr : &(*this)(i-1, j-1));
            Tile *tLeft = (_oob(i-1, j) ? nullptr : &(*this)(i-1, j));

            Tile tBest;
            double minDist = INFINITY;
            for(int k = 0; k < NB_CHOICES; k++)
            {
                Tile t2(orig.get_random_tile(_tileSize, _tileFringe), _tileFringe);
                double d2 = t2.dst(tLeft, tUpLeft, tUp);

                if(d2 < minDist)
                {
                    minDist = d2;
                    tBest = t2;
                }

            }
            (*this)(i, j) = tBest;

            _vertical_scissor_on_tile(i, j);
            _horizontal_scissor_on_tile(i, j);
        }
    }
}

// ============================================================
// 	PNG manipulation
// ============================================================
void Canvas::write(const std::string& filename)
{
    unsigned char* raw = _to_raw();
    raw[3] = 255;
    raw[4] = 255;
    raw[5] = 255;

    raw[3*_nTileWidth*_tileSize] = 0;
    raw[3*_nTileWidth*_tileSize + 1] = 255;
    raw[3*_nTileWidth*_tileSize + 2] = 0;

    Image_utils::write(filename, raw, _nTileWidth*_tileSize, _nTileHeight*_tileSize);
    delete [] raw;
}



// ============================================================
// 	Destructor & operators
// ============================================================

Tile& Canvas::operator() (int row, int col)
{
    return this->_data[_index(row, col)];
}

Canvas::~Canvas()
{
    delete [] _data;
}

// ----------------------------------------------------------------------
// 		PRIVATE FUNCTIONS
// ----------------------------------------------------------------------

// ============================================================
// 	Misc & Index
// ============================================================
bool Canvas::_oob(int x, int y) const {
    return (x > this->_nTileWidth || y > this->_nTileHeight || x < 0 || y < 0);
}

int Canvas::_index(int x, int y) const
{
    if(_oob(x, y))
        throw std::runtime_error("Canvas subscript index out of bounds: (" + std::to_string(x) + ", " + std::to_string(y) + ") vs (" + std::to_string(_nTileWidth) + ", " + std::to_string(_nTileHeight) + ")");
    return _nTileWidth*y + x;

}

// ============================================================
// 	Write image
// ============================================================

unsigned char *Canvas::_to_raw()
{
    auto raw = new unsigned char[_nTileWidth * _nTileHeight * _tileSize * _tileSize * 3];
    for(int i = 0; i < _nTileWidth; i ++)
    {
        for(int j = 0; j < _nTileHeight; j ++)
        {
            // Chaque tuile prends _tileSize * _tileSize * 3 places dans le tableau
            _store_ij_tile(raw, i, j);
        }
    }
    return raw;
}

void Canvas::_store_ij_tile(unsigned char *raw, int i, int j)
{
    int id0 = 3*(j*_tileSize*_nTileWidth + i)*_tileSize;
    auto localRaw = new unsigned char[(2*_tileFringe + _tileSize) * (2*_tileFringe + _tileSize) * 3];
    _data[_index(i, j)].to_raw(localRaw);

    int picWidth = _tileSize * _nTileWidth;

    for(int k = 0; k < _tileSize; k++)
        for(int l = 0; l < _tileSize; l++)
        {
            int rpkl = id0 + 3*(l*picWidth + k);
            int pkl = 3*((2*_tileFringe + _tileSize) * _tileFringe
                    + (2*_tileFringe + _tileSize) * l
                    + _tileFringe + k);
            raw[rpkl] = localRaw[pkl];
            raw[rpkl+1] = localRaw[pkl+1];
            raw[rpkl+2] = localRaw[pkl+2];
        }

    delete [] localRaw;

}

// ============================================================
// 	Scissor
// ============================================================


void Canvas::_vertical_scissor_on_tile(int i, int j)
{
    if(i == 0)
        return;
    // For now, we only do vertical scissor, only with the tile on the left of the current one


    int distsize = 2*_tileFringe * (_tileSize + 2*_tileFringe);
    std::vector<double> dist(2*_tileFringe * (_tileSize + 2*_tileFringe));
    std::vector<int> map(2*_tileFringe * (_tileSize + 2*_tileFringe));

    auto fringe_idx = [this](int a, int b)
            {
                if (a < 0 || a >= 2*_tileFringe || b < 0 || b >= 2*_tileFringe + _tileSize)
                    return -1;
                else
                    return a + b*2*_tileFringe;
            };
    auto safe_dst = [&dist, distsize](int i){ return ((i >= 0 && i < distsize) ? dist[i] : INFINITY); };


    for(int u = 0; u < distsize; u++)
    {
        dist[u] = INFINITY;
        map[u] = -1;
    }

    // Set bottom row at 0
    for(int x =0; x < 2*_tileFringe; x++)
    {
        int botId = fringe_idx(x, 2*_tileFringe + _tileSize - 1);
        dist[botId] = 0;
        map[botId] = botId;

    }

    for(int y = 2*_tileFringe + _tileSize  - 2; y >= 0; y--)
    {
        for(int x = 0; x < 2*_tileFringe; x++)
        {
            double exy = _pxl_distance_in_vertical_fringe(i, j, {x, y});
            int l = x-1;
            int m = x+1;
            int k = y+1;

            double elk = safe_dst(fringe_idx(l, k));
            double exk = safe_dst(fringe_idx(x, k));
            double emk = safe_dst(fringe_idx(m, k));

	    // We select the tile minimizing the weight.
	    // As double always round in the same direction, 
	    // the equality test here is safe
            double mine = std::min(std::min(elk, exk), emk);
            if(mine < INFINITY)
            {
                if(elk == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(l, k);
                    dist[fringe_idx(x, y)] = exy + elk;
                }
                if(exk == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(x, k);
                    dist[fringe_idx(x, y)] = exy + exk;
                }
                if(emk == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(m, k);
                    dist[fringe_idx(x, y)] = exy + emk;
                }
            }
        }
    }

    int minIdx = 0;
    for(int u = 0; u < 2*_tileFringe; u++)
    {
        if(dist[u] < dist[minIdx])
            minIdx = u;
    }

     int endPt = minIdx;
    _vertical_render_tile(i, j, map, endPt);
}

void Canvas::_horizontal_scissor_on_tile(int i, int j)
{
    // We apply here an horizontal cut exactely the same way we did the vertical one
    if(j == 0)
        return;


    int maxX = 2*_tileFringe + _tileSize;
    int distsize = 2*_tileFringe * (_tileSize + 2*_tileFringe);
    std::vector<double> dist(distsize);
    std::vector<int> map(distsize);

    auto fringe_idx = [this](int a, int b)
            {
                if (a < 0 || a >= 2*_tileFringe + _tileSize || b < 0 || b >= 2*_tileFringe)
                    return -1;
                else
                    return a + b*(2*_tileFringe + _tileSize);
            };
    auto safe_dst = [&dist, distsize](int i){ return ((i >= 0 && i < distsize) ? dist[i] : INFINITY); };


    for(int u = 0; u < distsize; u++)
    {
        dist[u] = INFINITY;
        map[u] = -1;
    }

    // Set bottom row at 0
    for(int y =0; y < 2*_tileFringe; y++)
    {
        int botId = fringe_idx(0, y);
        dist[botId] = 0;
        map[botId] = botId;
    }

    for(int x = 1; x < maxX; x++)
    {
        for(int y = 0; y < 2*_tileFringe; y++)
        {
            double exy = _pxl_distance_in_horinzontal_fringe(i, j, {x, y});
            int l = y-1;
            int m = y+1;
            int k = x-1;

            double ekl = safe_dst(fringe_idx(k, l));
            double eky = safe_dst(fringe_idx(k, y));
            double ekm = safe_dst(fringe_idx(k, m));

	    // We select the tile minimizing the weight.
	    // As double always round in the same direction, 
	    // the equality test here is safe
            double mine = std::min(std::min(ekl, eky), ekm);
            if(mine < INFINITY)
            {
                if(ekl == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(k, l);
                    dist[fringe_idx(x, y)] = exy + ekl;
                }
                if(eky == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(k, y);
                    dist[fringe_idx(x, y)] = exy + eky;
                }
                if(ekm == mine)
                {
                    map[fringe_idx(x, y)] = fringe_idx(k, m);
                    dist[fringe_idx(x, y)] = exy + ekm;
                }
            }
        }
    }

    int minIdx = fringe_idx(maxX-1, 0);
    for(int u = 1; u < 2*_tileFringe; u++)
    {
        int pu = fringe_idx(maxX-1, u);
        if(dist[pu] < dist[minIdx])
            minIdx = pu;
    }

     int endPt = minIdx;
    _horizontal_render_tile(i, j, map, endPt);

}

void Canvas::_horizontal_render_tile(int i, int j, const std::vector<int> &map, int endPt)
{
    int maxX = 2*_tileFringe + _tileSize;
    int currentPt = endPt;
    int y = currentPt / maxX; 

    for(int x = maxX-1; x >= 0; x--)
    {
        _copy_column(i, j, {x, y});

        currentPt = map[currentPt];
        y = currentPt / maxX;
    }
}

void Canvas::_vertical_render_tile(int i, int j, const std::vector<int> &map, int endPt)
{
    int mapsize = 2*_tileFringe;
    int currentPt = endPt;
    int x = currentPt % mapsize;
    for(int y = 0; y < 2*_tileFringe + _tileSize; y++)
    {
        _copy_line(i, j, {x, y});

        currentPt = map[currentPt];
        x = currentPt % mapsize;
    }
}

void Canvas::_copy_line(int i, int j, Canvas::Point p)
{
	int x = std::get<0>(p);
	int y = std::get<1>(p);

	// If the point has an abscisse lesser than _tileFringe, then the left tile has
	// to be modified
	// If the abscisse is greater than, then the current one has to be modified.

	if(!_oob(i-1, j))
	{
		if(x < _tileFringe)
		{
			for(int u = x; u < _tileFringe; u++)
			{
				int xshift = _tileSize;
				(*this)(i-1, j)(u + xshift, y) = (*this)(i, j)(u, y);
			}
		}
		else
		{
			for(int u = _tileFringe; u <= x; u++)
			{
				int xshift =_tileSize;
				(*this)(i, j)(u, y) = (*this)(i-1, j)(u+xshift, y);
			}
		}
	}
}


void Canvas::_copy_column(int i, int j, Canvas::Point p)
{
	int x = std::get<0>(p);
	int y = std::get<1>(p);

	if(!_oob(i, j-1))
	{
		if(y < _tileFringe)
		{
			for(int u = y; u < _tileFringe; u++)
			{
				int yshift = _tileSize;
				(*this)(i, j-1)(x, u+yshift) = (*this)(i, j)(x, u);
			}
		}
		else
		{
			for(int u = _tileFringe; u <= y; u++)
			{
				int yshift =_tileSize;
				(*this)(i, j)(x, u) = (*this)(i, j-1)(x, u+yshift);
			}
		}
	}
}

double Canvas::_pxl_distance_in_horinzontal_fringe(int i, int j, Canvas::Point p1)
{   int x = std::get<0>(p1);
    int y = std::get<1>(p1);

    if((x >= 2*_tileFringe && y >= 2*_tileFringe))
        return INFINITY;


    //top margin
    if(y < 2*_tileFringe)
    {
        if(_oob(i, j-1))
            return INFINITY;

        int yshift = _tileSize;
        return (*this)(i, j-1)(x, y + yshift).dst((*this)(i, j)(x, y));
    }

    return INFINITY;
}

double Canvas::_pxl_distance_in_vertical_fringe(int i, int j, Point p1)//, Point p2)
{
    // TODO: REDUCE THIS FUNCTION
    int x1 = std::get<0>(p1);
    int y1 = std::get<1>(p1);

    if((x1 >= 2*_tileFringe && y1 >= 2*_tileFringe))
        return INFINITY;


    // Left margin
    if(x1 < 2*_tileFringe && y1 >= 2*_tileFringe)
    {
        if(_oob(i-1, j))
            return INFINITY;

        int xshift = _tileSize;
        return (*this)(i-1, j)(x1 + xshift, y1).dst((*this)(i, j)(x1, y1));
    }

    //top margin
    if(x1 >= 2*_tileFringe && y1 < 2*_tileFringe)
    {
        if(_oob(i, j-1))
            return INFINITY;

        int yshift = _tileSize;
        return (*this)(i, j-1)(x1, y1 + yshift).dst((*this)(i, j)(x1, y1));
    }

    // Mixed part  : upper corner
    if(x1 <= 2*_tileFringe && y1 <= 2*_tileFringe)
    {
        if(_oob(i-1, j))
            return INFINITY;

        int xshift = _tileSize;
        return (*this)(i-1, j)(x1 + xshift, y1).dst((*this)(i, j)(x1, y1));

    }

    return INFINITY;
}




