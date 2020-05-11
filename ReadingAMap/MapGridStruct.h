#ifndef _MAPGRID_H
#define _MAPGRID_H

#include "pch.h"
#include "Coord.h"
#include <vector>

struct MapGrid //This class stores everything the map needs to know to keep a consistent size and state and end points
{
	std::vector<std::vector<int>> MapTiles;

	int Width = 0;
	int Height = 0;

	Coord POO = {0, 0}; //Coordinate Point Of Origin
	Coord POE = {0, 0}; //Coordinare Point Of End
};

#endif // !_MAPGRID_H
