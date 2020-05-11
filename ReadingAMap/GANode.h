#ifndef _GANODE_H
#define _GANODE_H

#include "pch.h"
#include "Coord.h"
#include "MapGridStruct.h"

struct GANode // Same as the A* node but less complicated, where is it, what did it come from and is it okay to continue?
{
	Coord Pos = {};
	int ParentX;
	int ParentY;

	static bool isValid(MapGrid _Map, int x, int y);
};

#endif
