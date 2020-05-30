#ifndef PICTURE_H
#define PICTURE_H
#include "pixel.h"

//#include <cstdlib>

#include "random.h"
#include "Image_utils.h"

// Fancy wrapper for a single array of (int, int, int) to be represented as a 2D array of Pxl
class Picture
{
    public:
        Picture();
        Picture(int width, int height);
        explicit Picture(const std::string& filename);
        Picture(unsigned char* rawdata, int width, int height);

	// ============================================================
	// 	Getters and Setters
	// ============================================================
        Pxl<int> get_pxl(int i, int j) const;
        Picture get_tile(int i, int j, int side, int fringe) const;

        int get_width() const;
        int get_height() const;

        void set_tile(Picture source, int i, int j);

        Picture get_random_tile(int side, int fringe) const;

	// ============================================================
	// 	PNG Manip
	// ============================================================
        void to_raw(unsigned char* dest);
        void write(std::string& filename);


	// ============================================================
	// 	Operators & Destructors
	// ============================================================
        Pxl<int>& operator() (int row, int col);
        ~Picture();


    protected:
        int _width{}, _height{};
        Pxl<int>* _data{};


    private:
	// Same as Canvas, we use some little function to ease 2D-1D conversion
        bool _oob(int width, int height) const;
        int _index(int x, int j) const;
        void _construct(Image_utils &iu);

};
#endif
