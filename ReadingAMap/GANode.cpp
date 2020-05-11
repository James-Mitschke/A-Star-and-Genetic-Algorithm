#include "pch.h"
#include "GANode.h"

bool GANode::isValid(MapGrid _Map, int x, int y)
{
	if (x < 0 || y < 0 || x >= (_Map.Width) || y >= (_Map.Height)) //Is the node in map bounds? No, then don't use it
	{
		return false;
	}

	if (_Map.MapTiles[y][x] == 1) //Is the node a wall? Yes, then don't use it
	{
		return false;
	}

	else //Is the node anything else? Yes, good, don't let your dreams be dreams
	{
		return true;
	}
}