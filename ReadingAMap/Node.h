#ifndef _NODE_H
#define _NODE_H

#include "pch.h"
#include "Coord.h"
#include "MapGridStruct.h"

struct Node //This is a class to pre-define everything a Node can and should have as well as the functions it might need to call
{
	Coord Pos = {};
	int ParentX;
	int ParentY;
	int gCost;
	double hCost;
	double fCost;

	static bool isValid(MapGrid _Map, int x, int y); //Such as if the node's position is possible

	static bool ClosedValid(std::vector<Node> _ClosedList, int x, int y); //If it's on the closed list

	static bool OpenValid(std::vector<Node> _OpenList, int x, int y); //If it's on the open list

	void CalcH(MapGrid _Map, Coord); //And what its H Cost is 
};


#endif // !_NODE_H

