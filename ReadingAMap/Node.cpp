#include "pch.h"
#include "Node.h"

bool Node::isValid(MapGrid _Map, int x, int y) //The node class' functions
{
	if (x < 0 || y < 0 || x >= (_Map.Width) || y >= (_Map.Height)) //Is the node on the map? If no then return false
	{
		return false;
	}

	if (_Map.MapTiles[y][x] == 1) //Is the node a wall? If yes then return false
	{
		return false;
	}

	else //Otherwise yes, you can move onto it
	{
		return true;
	}
}

void Node::CalcH(MapGrid _Map, Coord) //H cost dictates how far away the current node is from the end of the map
{
	hCost = 100 * (sqrt((Pos.X - _Map.POE.X)*(Pos.X - _Map.POE.X) + (Pos.Y - _Map.POE.Y)*(Pos.Y - _Map.POE.Y)));
}

bool Node::ClosedValid(std::vector<Node> _ClosedList, int x, int y) //Simply is the node on the closed list? If yes then the node is not a valid successor, if no then it is
{
	int ClosedListSize = _ClosedList.size();

	if (ClosedListSize > 0)
	{
		bool ClosedCheck = false;

		if (ClosedCheck == false)
		{
			for (int C = 0; C < ClosedListSize; C++)
			{
				int BoolTempPosX = _ClosedList[C].Pos.X;
				int BoolTempPosY = _ClosedList[C].Pos.Y;

				if (BoolTempPosX == x && BoolTempPosY == y)
				{
					return false;
				}
			}

			ClosedCheck = true;
		}
		
		if (ClosedCheck == true)
		{
			return true;
		}
	}

	else
	{
		return true;
	}
}

bool Node::OpenValid(std::vector<Node> _OpenList, int x, int y) //Same as the closed list but with the open list
{
	int OpenListSize = _OpenList.size();

	if (OpenListSize > 0)
	{
		bool OpenCheck = false;

		if (OpenCheck == false)
		{
			for (int C = 0; C < OpenListSize; C++)
			{
				int BoolTempPosX = _OpenList[C].Pos.X;
				int BoolTempPosY = _OpenList[C].Pos.Y;

				if (BoolTempPosX == x && BoolTempPosY == y)
				{
					return false;
				}
			}

			OpenCheck = true;
		}

		if (OpenCheck == true)
		{
			return true;
		}
	}

	else
	{
		return true;
	}
}