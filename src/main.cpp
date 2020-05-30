//
// Created by maxime on 06/03/19.
//

#include "Picture.h"
#include "Canvas.h"

#include <cstdlib>
#include <iostream>
#include <ctime>

using namespace std;

int main(int argc, char* argv[])
{
    int tileSide, tileFringe;
    string brutfile;

    if(argc == 4)
    {
        brutfile = argv[1];
        tileSide = stoi(argv[2]);
        tileFringe = stoi(argv[3]);
    }
    else
    {
        std::cerr << "Wrong parameters. Usage: ./<exec> fileTrunk tileSide tileFringe\nWARNING: fileTrunk corresponds to the file data/fileTrunk.png" << std::endl;
        exit(1);
    }

    std::cout << "Loading pic..." << std::endl;
    Picture pic("data/" + brutfile + ".png");
    Canvas canvas(10,5, tileSide, tileFringe);

    // We can do it in two steps
    //canvas.fill_with_random_tiles(pic);
    //canvas.fill_with_best_random_tiles(pic);
    //canvas.scissor();
    std::cout << "Filling and cutting new Canvas..." << std::endl;
    // Or in only one. There is little difference, put aside the lesser memory usage when done in one go
    canvas.fill_and_cut(pic);


    canvas.write(brutfile + "_" + std::to_string(tileSide) + "_" + std::to_string(tileFringe) + ".png");
    std::cout << "File written." << std::endl;

    return 0;
}
