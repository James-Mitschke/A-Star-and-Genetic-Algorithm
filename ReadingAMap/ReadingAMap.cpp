// ReadingAMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "Coord.h"
#include "MapGridStruct.h"
#include "stdlib.h"
#include "SDL.h"
#include "Windows.h"
#include "direct.h"
#include "Node.h"
#include "GANode.h"

#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stack>
#include <algorithm>
#include <ctime>
#include <utility>
#include <random>

#define RECTSIZE 100;

void initMap(MapGrid *_Map);
void drawMap(MapGrid _Map, SDL_Renderer *_renderer);

void AStarFindPath(std::vector <Node> &ClosedList, struct Node &Current, MapGrid _Map);
void AStarDrawPath(std::vector<Node> &ClosedList, struct Node &Current, SDL_Renderer *_renderer);

void GAFindPath(std::vector<GANode> &PathToEnd, struct GANode &GACurrent, MapGrid _Map);
void GADrawPath(std::vector<GANode> &PathToEnd, struct GANode &GACurrent, SDL_Renderer *_renderer);

//The above is used to define the functions made below so that I can call them in the main function and it knows what to look for

bool UserChoice = 1;
std::string UserInput = "";

struct MapGrid Map;

int main(int argc, char *argv[]) //This is used to call all of the functions in the correct order
{
	while (true)
	{
		SDL_Init(SDL_INIT_VIDEO);

		MapGrid Map;

		struct Node Current;
		std::vector<Node> ClosedList;

		struct GANode GACurrent;
		std::vector<GANode> PathToEnd;

		initMap(&Map);

		std::cout << "What program would you like to run? ";
		std::cin >> UserInput;

		//This allows the user a degree of control over the program making it more flexible and easier to use

		if (UserInput == "A*")
		{
			SDL_Window *window = SDL_CreateWindow("A* Algorithm Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_RESIZABLE);
			SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

			SDL_Event Refresh;
			SDL_PollEvent(&Refresh);

			//These create an SDL window for rendering the maze files and path taken by the appropriate algorithm for display

			UserChoice = 1;

			while (Refresh.type != SDL_QUIT) //This just prevents the window from closing straight away after drawing the maze and path so the user has time to witness the results
			{
				drawMap(Map, renderer);
				AStarFindPath(ClosedList, Current, Map);
				AStarDrawPath(ClosedList, Current, renderer);
				Sleep(10000);
				Refresh.type = SDL_QUIT;
			}

			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();

			//The window is then destroyed to allow control back to the console and to free up resources on the user's computer
		}

		else if (UserInput == "GA")
		{
			SDL_Window *window = SDL_CreateWindow("Genetic Algorithm Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1024, 1024, SDL_WINDOW_RESIZABLE);
			SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_PRESENTVSYNC);

			SDL_Event Refresh;
			SDL_PollEvent(&Refresh);

			UserChoice = 2;

			while (Refresh.type != SDL_QUIT)
			{
				drawMap(Map, renderer);
				GAFindPath(PathToEnd, GACurrent, Map);
				GADrawPath(PathToEnd, GACurrent, renderer);
				Sleep(10000);
				Refresh.type = SDL_QUIT;
			}

			SDL_DestroyRenderer(renderer);
			SDL_DestroyWindow(window);
			SDL_Quit();
		} //Again this is to allow the user to view the solution, and then free up resources

		else
		{
			UserChoice = 0;
			return 0;
		} //This allows the user to exit the program from within by typing anything other than what you want to run, it's done this way to make it easy for a user to quit the program
	}
}

void initMap(MapGrid *_Map) //This is the map initialization function used for getting the map file and reading it properly
{
	std::ifstream inFile;
	std::string CurrentLine;

	if (UserChoice != 0)
	{
		std::cout << "Please input a map file name from the maps folder: ";
		std::cin >> UserInput;
		std::cout << std::endl;
		std::cout << "Input accepted!" << std::flush;
		Sleep(2000);
		system("CLS");
		_chdir("Map Files\\");

		inFile.open(UserInput.c_str());

		//After some research I found out how to open files from a specific directory given only the name, this was to make it easy for the user to type in what map they want to load

		if (inFile)
		{
			while (std::getline(inFile, CurrentLine))
			{
				std::stringstream MapValues(CurrentLine);

				//Test code to check if the program can read the map file inputs correctly
				//std::cout << MapValues.str() << std::endl;


				if (!(MapValues >> (*_Map).Width))
				{
					std::cout << "Could not read width of map!" << std::endl;
				}

				if (!(MapValues >> (*_Map).Height))
				{
					std::cout << "Could not read height of map!" << std::endl;
				}

				//These check if the map file contains a readable size for generating, and if not will throw an error

				int PresentTile = 0;
				std::vector<int> CurTempVec; //Defines a temporary vector for passing lines of the map into the map vector 

				for (int h = 0; h < (*_Map).Height; h++)
				{
					for (int w = 0; w < (*_Map).Width; w++)
					{
						if ((MapValues >> PresentTile))
						{
							CurTempVec.push_back(PresentTile);

							if (PresentTile == 2)
							{
								(*_Map).POO = { w, h };
							}

							if (PresentTile == 3)
							{
								(*_Map).POE = { w, h };
							}
						}
					}
					(*_Map).MapTiles.push_back(CurTempVec);
					CurTempVec.clear();
				}

				//This code is to push, tile by tile, what is on the map into a temporary vector for storage

				/* Test code to check if the vector map is created from the map file correctly
				for (int h = 0; h < (*_Map).Height; h++)
				{
					for (int w = 0; w < (*_Map).Width; w++)
					{
						std::cout << (*_Map).MapTiles[h][w];
					}
					std::cout << std::endl;
				} */

				inFile.close(); //This closes the file because why would we keep it open and expend valuable resources?
			}
		}

		else
		{
			std::cout << "Incorrect name, program now exiting!";
			exit(1);
		} //This causes the program to exit on an incorrect map name, prevents having to retype a map name in if the user actually wanted to exit
	} 

	else
	{
		exit(1);
	}
}

void drawMap(MapGrid _Map, SDL_Renderer *_renderer) //This function will draw the map to the screen for viewing
{
	SDL_SetRenderDrawColor(_renderer, 255, 255, 255, 255);
	SDL_RenderClear(_renderer);

	SDL_Rect indTile;
	indTile.w = RECTSIZE;
	indTile.h = RECTSIZE;

	for (int h = 0; h < (_Map).Height; h++)
	{
		for (int w = 0; w < (_Map).Width; w++)
		{
			indTile.x = w * RECTSIZE;
			indTile.y = h * RECTSIZE;
			SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
			SDL_RenderDrawRect(_renderer, &indTile);

			if (_Map.MapTiles[h][w] == 0)
			{
				indTile.x = w * RECTSIZE;
				indTile.y = h * RECTSIZE;
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				SDL_RenderDrawRect(_renderer, &indTile);
			}

			if (_Map.MapTiles[h][w] == 1)
			{
				indTile.x = w * RECTSIZE;
				indTile.y = h * RECTSIZE;
				SDL_SetRenderDrawColor(_renderer, 0, 0, 0, 255);
				SDL_RenderFillRect(_renderer, &indTile);
			}

			if (_Map.MapTiles[h][w] == 2)
			{
				indTile.x = w * RECTSIZE;
				indTile.y = h * RECTSIZE;
				SDL_SetRenderDrawColor(_renderer, 0, 255, 0, 255);
				SDL_RenderFillRect(_renderer, &indTile);
			}

			if (_Map.MapTiles[h][w] == 3)
			{
				indTile.x = w * RECTSIZE;
				indTile.y = h * RECTSIZE;
				SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);
				SDL_RenderFillRect(_renderer, &indTile);
			}
		}
	}
	
	SDL_RenderPresent(_renderer);

	//It's quite simple code, it just colours the map based on the number in the file, 0 is movable tiles, 1 is a wall, 2 is the path start and 3 is the path finish
}

void AStarFindPath(std::vector<Node> &ClosedList, struct Node &Current, MapGrid _Map) //This function finds the appropriate path for drawing later and stores it
{
	std::vector<Node> OpenList;

	Current.Pos.X = _Map.POO.X;
	Current.Pos.Y = _Map.POO.Y;
	Current.ParentX = -1;
	Current.ParentY = -1;

	//The first node's parents need to be something not on the map so that we can recognise it as the start point easily later when drawing the line

	struct Node NewNode;

	Node TestCurrent;

	Current.gCost = 0;
	Current.CalcH(_Map, Current.Pos);
	Current.fCost = Current.gCost + Current.hCost;

	//Hopefully you've read into A* Algorithm to know how it works but if not, each tile has 3 costs, a Gcost (Cost of tile from the start), an Hcost (cost of tile to the end)
	// and finally an Fcost which is the total of them. A* will always attempt the lowest Fcost possible before choosing any other tile for the shortest path

	Node NewLowestFCostNode = Current;

	bool Test = true;
	bool onClosedList = false;
	bool DestFound = false;

	do
	{
		double LowFCost = 9999; //This is for storing the lowest Fcost later, the first value needs something to beat so setting this higher than possible is the optimal solution

		for (int newPosY = -1; newPosY <= +1; newPosY++)
		{
			for (int newPosX = -1; newPosX <= +1; newPosX++)
			{
				int TempPosX = Current.Pos.X + newPosX;
				int TempPosY = Current.Pos.Y + newPosY;
				if (NewNode.isValid(_Map, TempPosX, TempPosY)) //This loop here generates every possible successor to move onto from the current tile and check if it's valid
				{
					if (TempPosX == _Map.POE.X && TempPosY == _Map.POE.Y)
					{
						ClosedList.push_back(Current);
						NewNode.Pos.X = TempPosX;
						NewNode.Pos.Y = TempPosY;
						NewNode.ParentX = Current.Pos.X;
						NewNode.ParentY = Current.Pos.Y;
						Current = NewNode;
						ClosedList.push_back(Current);
						DestFound = true;
						break;
					} //If the node being checked happens to be the end tile it will add the current node onto the closed list and push the new node onto it too before generating the path

					if (TempPosX == Current.Pos.X && TempPosY == Current.Pos.Y)
					{
						continue;
					} //This segment just prevents the current tile from being generated as a successor

					if (NewNode.ClosedValid(ClosedList, TempPosX, TempPosY) == true) //This prevents nodes on the closed list from being put onto the path twice
					{

						if ((newPosX == -1 && newPosY == -1) || (newPosX == -1 && newPosY == 1) || (newPosX == 1 && newPosY == -1) || (newPosX == 1 && newPosY == 1))
						{
							//This if for new tiles diagonal from the current tile

							NewNode.Pos = { TempPosX, TempPosY };

							if (NewNode.OpenValid(OpenList, NewNode.Pos.X, NewNode.Pos.Y) == true) //This checks if the new tiles is on the open list, and if not then it puts it onto the list
							{
								NewNode.gCost = Current.gCost + 14;
								NewNode.CalcH(_Map, NewNode.Pos);
								NewNode.fCost = NewNode.gCost + NewNode.hCost;
								NewNode.ParentX = Current.Pos.X;
								NewNode.ParentY = Current.Pos.Y;
								OpenList.push_back(NewNode);

								if (NewNode.fCost < LowFCost) //If it happens to have the lowest Fcost it will be stored for potentially being put on the closed list
								{
									NewLowestFCostNode = NewNode;
									LowFCost = NewNode.fCost;
								}

								continue;
							}

							int OpenListSize = OpenList.size();

							for (int C = 0; C < OpenListSize; C++) //If the node was on the open list, this finds it and checks if the new path to it is shorter than the current path to it
							{
								int BoolTempPosX = OpenList[C].Pos.X;
								int BoolTempPosY = OpenList[C].Pos.Y;

								if (BoolTempPosX == NewNode.Pos.X && BoolTempPosY == NewNode.Pos.Y)
								{
									NewNode.gCost = OpenList[C].gCost;
									NewNode.CalcH(_Map, NewNode.Pos);
									NewNode.fCost = NewNode.gCost + NewNode.hCost;
									NewNode.ParentX = OpenList[C].ParentX;
									NewNode.ParentY = OpenList[C].ParentY;
								} //If it is, it's restored on the open list with new parents
							}

							int CurrentTestgCost = Current.gCost + 14; //As the tested node is diagonal from the current node, we make a temporary expected cost

							if (CurrentTestgCost < NewNode.gCost) //And then here check if the expected cost is less than the actual cost
							{
								int PosInVector = 0;
								int OpenListSize = OpenList.size();

								for (int C = 0; C < OpenListSize; C++) //This is so that if the new path to the node is shorter, we can find and update the path to it on the open list
								{
									int BoolTempPosX = OpenList[C].Pos.X;
									int BoolTempPosY = OpenList[C].Pos.Y;

									if (BoolTempPosX == Current.Pos.X && BoolTempPosY == Current.Pos.Y) //This is to make sure we find the absolute shortest path to the end
									{
										OpenList.erase(OpenList.begin() + C);
										NewNode.ParentX = Current.Pos.X;
										NewNode.ParentY = Current.Pos.Y;
										NewNode.gCost = Current.gCost + 10;
										NewNode.CalcH(_Map, NewNode.Pos);
										NewNode.fCost = NewNode.gCost + NewNode.hCost;
										OpenList.push_back(NewNode); //By overwriting new nodes with their absolute shortest path to them

										if (NewNode.fCost < LowFCost) //We then check if the new node has become the best node for moving forward towards the end
										{
											NewLowestFCostNode = NewNode;
											LowFCost = NewNode.fCost; //And make it the new lowest node if it is
										}

										continue;
									}
								}
							}
							
							if (NewNode.fCost < LowFCost) //Otherwise if the current node's path is better and it has the lowest cost, it becomes the new lowest node
							{
								NewLowestFCostNode = NewNode;
								LowFCost = NewNode.fCost;
							}
						}

						else //This is for all successors that are horizontal or vertical to the current node
						{
							NewNode.Pos = { TempPosX, TempPosY };

							if (NewNode.OpenValid(OpenList, NewNode.Pos.X, NewNode.Pos.Y) == true) //And it runs through the same checks as before but with non diagonals
							{
								NewNode.gCost = Current.gCost + 10;
								NewNode.CalcH(_Map, NewNode.Pos);
								NewNode.fCost = NewNode.gCost + NewNode.hCost;
								NewNode.ParentX = Current.Pos.X;
								NewNode.ParentY = Current.Pos.Y;
								OpenList.push_back(NewNode);

								if (NewNode.fCost < LowFCost)
								{
									NewLowestFCostNode = NewNode;
									LowFCost = NewNode.fCost;
								}

								continue;
							}

							int OpenListSize = OpenList.size();

							for (int C = 0; C < OpenListSize; C++)
							{
								int BoolTempPosX = OpenList[C].Pos.X;
								int BoolTempPosY = OpenList[C].Pos.Y;

								if (BoolTempPosX == NewNode.Pos.X && BoolTempPosY == NewNode.Pos.Y)
								{
									NewNode.gCost = OpenList[C].gCost;
									NewNode.CalcH(_Map, NewNode.Pos);
									NewNode.fCost = NewNode.gCost + NewNode.hCost;
									NewNode.ParentX = OpenList[C].ParentX;
									NewNode.ParentY = OpenList[C].ParentY;
								}
							}

							int CurrentTestgCost = Current.gCost + 10; //Meaning the cost to a new node is +10 from the current node

							if (CurrentTestgCost < NewNode.gCost)
							{
								int PosInVector = 0;
								int OpenListSize = OpenList.size();

								for (int C = 0; C < OpenListSize; C++)
								{
									int BoolTempPosX = OpenList[C].Pos.X;
									int BoolTempPosY = OpenList[C].Pos.Y;

									if (BoolTempPosX == Current.Pos.X && BoolTempPosY == Current.Pos.Y)
									{
										OpenList.erase(OpenList.begin() + C);
										NewNode.ParentX = Current.Pos.X;
										NewNode.ParentY = Current.Pos.Y;
										NewNode.gCost = Current.gCost + 10;
										NewNode.CalcH(_Map, NewNode.Pos);
										NewNode.fCost = NewNode.gCost + NewNode.hCost;
										OpenList.push_back(NewNode);

										if (NewNode.fCost < LowFCost)
										{
											NewLowestFCostNode = NewNode;
											LowFCost = NewNode.fCost;
										}

										continue;
									}
								}
							}

							if (NewNode.fCost < LowFCost)
							{
								NewNode.ParentX = Current.ParentX;
								NewLowestFCostNode = NewNode;
								LowFCost = NewNode.fCost;
							}
						}
					}
				}
			}
		}

		if (DestFound == false) //If the destination hasn't been found
		{
			ClosedList.push_back(Current); //Pushes the current node onto the closed list
			Current = NewLowestFCostNode; //Sets the current as the new lowest node
			NewNode = {}; //And resets all the new node data to avoid confusion
		}
	}

	while ((!OpenList.empty()) && DestFound == false); //This runs everything a minimum of once until the destination has been found
}

void AStarDrawPath(std::vector<Node> &ClosedList, struct Node &Current, SDL_Renderer *_renderer)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);

	SDL_Rect PathTile;
	PathTile.w = RECTSIZE;
	PathTile.h = RECTSIZE;

	std::vector<Node> thePath;

	Node NewCurrent;

	int ClosedListSize = ClosedList.size();

	for (int C = 0; C < ClosedListSize; C++) //This pushes the path found in the previous function onto a new list in order from start to end
	{
		NewCurrent = *(ClosedList.rbegin() + C);

		if ((Current.ParentX == NewCurrent.Pos.X && Current.ParentY == NewCurrent.Pos.Y) && (Current.Pos.X != -1 && Current.Pos.Y != -1))
		{
			thePath.push_back(Current);
			Current = NewCurrent;
		}
	}

	int thePathSize = thePath.size();

	for (int C = 0; C < thePathSize; C++) //And then for as long as the list is, will output it visually on the map
	{
		NewCurrent = *(thePath.rbegin() + C);
		PathTile.x = PathTile.w * NewCurrent.Pos.X;
		PathTile.y = PathTile.h * NewCurrent.Pos.Y;
		SDL_RenderFillRect(_renderer, &PathTile);
		SDL_RenderPresent(_renderer);
	}
}

void GAFindPath(std::vector<GANode> &PathToEnd, struct GANode &GACurrent, MapGrid _Map)
{
	//If you are unsure about what GA is I would recommend you take some time to read over it
	//In simple terms, think natural evolution, mutation and survival of the fittest

	int MoveLimit = 60;
	float MutationChance = 0.30;
	int PopulationSize = 8;
	float CrossoverChance = 0.50; //These are some basic variables needed for the whole function to work

	bool DestFound = false;
	bool OrigGen = false;

	std::vector<int> DirectionsList;
	std::vector<std::vector<int> > PopulationList;
	PopulationList.reserve(PopulationSize);
	std::vector<std::vector<int> > ChildrenList;
	ChildrenList.reserve(PopulationSize);
	std::vector<float> AllFitness;
	std::vector<int> CrossoverList;

	//This creates a lot of much needed 1D and 2D vectors (2D vectors are lists of lists)

	GACurrent.Pos = _Map.POO;
	GACurrent.ParentX = -1;
	GACurrent.ParentY = -1;
	
	for (int j = 0; j < PopulationSize; j++) //For the population size (Set to a minimum of 2, any higher than 8 eats resources) generate some random directions and store them as a list
	{
		std::random_device rand;
		std::default_random_engine generator(rand());
		std::uniform_int_distribution<int> distribution(1, 8);

		int A = 0;

		for (int i = 0; i < MoveLimit; i++) //For the move limit of each population (Set to play nicely with 10x10 grid sizes)
		{
			A = distribution(generator); //I figured this out by drawing out a 10x10 map with the longest possible path (back and forth) and counting the minimum turns from start to end
			DirectionsList.push_back(A);
		}

		PopulationList.push_back(DirectionsList);
		DirectionsList.clear();
	}

	do //Runs a minimum of once and then repeats whilst the while condition is met
	{
		for (int i = 0; i < PopulationSize; i++) //For the population size, set the initial parents like in A*
		{
			GACurrent.ParentX = -1;
			GACurrent.ParentY = -1;
			GACurrent.Pos = _Map.POO;

			for (int j = 0; j < MoveLimit; j++) //For the move limit, perform these checks
			{
				if (GACurrent.Pos.X == _Map.POE.X && GACurrent.Pos.Y == _Map.POE.Y) //If the current position is the end position, in which case the path is found
				{
					DestFound = true;
					break;
				}

				if (PopulationList[i][j] == 1) //Otherwise if the current position is above (below? I can't remember how it reads files but all basis are covered anyway)
				{
					int tempPosY = GACurrent.Pos.Y + 1;

					if (GACurrent.isValid(_Map, GACurrent.Pos.X, tempPosY) == true) //Then check if the new position would be valid to move to
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent); //And if it is, add it to a potential path to the end
					}
				}

				if (PopulationList[i][j] == 2)
				{
					int tempPosX = GACurrent.Pos.X + 1;
					int tempPosY = GACurrent.Pos.Y + 1;

					if (GACurrent.isValid(_Map, tempPosX, tempPosY) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 3)
				{
					int tempPosX = GACurrent.Pos.X + 1;

					if (GACurrent.isValid(_Map, tempPosX, GACurrent.Pos.Y) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 4)
				{
					int tempPosX = GACurrent.Pos.X + 1;
					int tempPosY = GACurrent.Pos.Y - 1;

					if (GACurrent.isValid(_Map, tempPosX, tempPosY) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 5)
				{
					int tempPosY = GACurrent.Pos.Y - 1;

					if (GACurrent.isValid(_Map, GACurrent.Pos.X, tempPosY) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 6)
				{
					int tempPosX = GACurrent.ParentX - 1;
					int tempPosY = GACurrent.Pos.Y - 1;

					if (GACurrent.isValid(_Map, tempPosX, tempPosY) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 7)
				{
					int tempPosX = GACurrent.Pos.X - 1;

					if (GACurrent.isValid(_Map, tempPosX, GACurrent.Pos.Y) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						PathToEnd.push_back(GACurrent);
					}
				}

				if (PopulationList[i][j] == 8) //All of these repeat like the first but in different directions
				{
					int tempPosX = GACurrent.Pos.X - 1;
					int tempPosY = GACurrent.Pos.Y + 1;

					if (GACurrent.isValid(_Map, tempPosX, tempPosY) == true)
					{
						GACurrent.ParentX = GACurrent.Pos.X;
						GACurrent.ParentY = GACurrent.Pos.Y;
						GACurrent.Pos.X = tempPosX;
						GACurrent.Pos.Y = tempPosY;
						PathToEnd.push_back(GACurrent);
					}
				}
			}

			if (DestFound == true) //If the destination is found then no need to continue
			{
				break;
			}

			float Fitness = 1.0 / (abs(GACurrent.Pos.X - _Map.POE.X) + abs(GACurrent.Pos.Y - _Map.POE.Y) + 1);
			AllFitness.push_back(Fitness);
			PathToEnd.clear(); //This check the fitness of the last path as an absolute value and stores it on a list of fitnesses
		}

		if (DestFound == true)
		{
			break;
		} //This just follows the last statement like this to ensure the program doesn't begin the next stage even after finding the path

		float Fit1 = AllFitness[0]; float Fit2 = AllFitness[1]; float Fit3 = AllFitness[2]; float Fit4 = AllFitness[3]; float Fit5 = AllFitness[4];
		float Fit6 = AllFitness[5]; float Fit7 = AllFitness[6]; float Fit8 = AllFitness[7]; //Here we call all 8 fitness values and assign them into floats

		float TotalFitness = Fit1 + Fit2 + Fit3 + Fit4 + Fit5 + Fit6 + Fit7 + Fit8; //Next we add them all up to get a total fitness

		float Ind1 = (Fit1 / TotalFitness) * 100; float Ind2 = (Fit2 / TotalFitness) * 100; float Ind3 = (Fit3 / TotalFitness) * 100; float Ind4 = (Fit4 / TotalFitness) * 100;
		float Ind5 = (Fit5 / TotalFitness) * 100; float Ind6 = (Fit6 / TotalFitness) * 100; float Ind7 = (Fit7 / TotalFitness) * 100; float Ind8 = (Fit8 / TotalFitness) * 100;
		//And then we divide the fitness by the total fitness to find out the percentage that fitness makes up, the bigger the percentage, the higher the chance of it being chosen in the next step

		std::default_random_engine Pasta(rand());
		std::uniform_real_distribution<float> SpinnerRand(0.0, 1.0); //We need a random number between 0 and 1

		for (int i = 0; i < 8; i++)
		{
			float Jackpot = SpinnerRand(Pasta);
			Jackpot = Jackpot * 100; //And then we times it by 100, this is to get a number like 45.7% instead of 45.7329849% which has no difference to the former apart from length

			if (Jackpot < Ind1)
			{
				int JackOne = 1;
				CrossoverList.push_back(JackOne);
			}

			else if (Jackpot < (Ind1 + Ind2))
			{
				int JackTwo = 2;
				CrossoverList.push_back(JackTwo);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3))
			{
				int JackThree = 3;
				CrossoverList.push_back(JackThree);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3 + Ind4))
			{
				int JackFour = 4;
				CrossoverList.push_back(JackFour);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3 + Ind4 + Ind5))
			{
				int JackFive = 5;
				CrossoverList.push_back(JackFive);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3 + Ind4 + Ind5 + Ind6))
			{
				int JackSix = 6;
				CrossoverList.push_back(JackSix);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3 + Ind4 + Ind5 + Ind6 + Ind7)) 
			{
				int JackSeven = 7;
				CrossoverList.push_back(JackSeven);
			}

			else if (Jackpot < (Ind1 + Ind2 + Ind3 + Ind4 + Ind5 + Ind6 + Ind7 + Ind8))
			{
				int JackEight = 8;
				CrossoverList.push_back(JackEight);
			} 

			//And now we pick 8 random paths (favouring the fittest of the paths but not excluding the others) and let them breed a new generation of children to find a correct path
			//We could pick the same path all 8 times if we're really unlucky, but the chances of that happening with an even chance of all paths is 0.00000596% to be precise
			//And say if one path accounts for 99% of the total fitness which is a very tiny chance of happening, the chance of picking the same path 8 times would be 92.27%
			//Or on a 10x10 grid, the maximum fitness a path can have is 60%, therefore picking it 8 times is possible 1.68% (Rounded) of the time
		}

		for (int i = 0; i < PopulationSize; i = i + 2) //For every 2 parents, we see if they breed the exact same children or a combination of their paths
		{
			float IsCrossedover = SpinnerRand(Pasta);

			if (IsCrossedover <= CrossoverChance) //If the RNG number is less than the threshold, they breed and make new children by simply splicing their paths half and half
			{
				int Silly = CrossoverList[i];
				int Silly2 = CrossoverList[i + 1];

				ChildrenList.push_back(PopulationList[i]);
				ChildrenList.push_back(PopulationList[i + 1]);

				for (int k = (MoveLimit / 2); k < MoveLimit; k++) //And making it a new path to test
				{
					ChildrenList[i][k] = PopulationList[i + 1][k];
					ChildrenList[i + 1][k] = PopulationList[i][k];
				}
			}

			else //Otherwise the children have the same unfortunate path as their parents, but maybe they'll breed better offspring
			{ 
				ChildrenList.push_back(PopulationList[i]);
				ChildrenList.push_back(PopulationList[i +1]);
			}
		}

		for (int i = 0; i < PopulationSize; i++) //Next we have a small chance of the offspring taking an entirely different direction the ones given by the parents
		{
			for (int j = 0; j < MoveLimit; j++)
			{
				float IsMutated = SpinnerRand(Pasta);

				if (IsMutated <= MutationChance)
				{
					int Mutated = rand() % 8 + 1;
					ChildrenList[i][j] = Mutated;
				}
			}
		} //This keeps the offspring from making the same mistakes as their parents, so in that small chance all the offspring remain the same, they will change somewhere along the line

		for (int i = 0; i < PopulationSize; i++) //Finally we kill off the parents and the children replace them
		{
			AllFitness.clear();
			TotalFitness = 0;
			GACurrent.Pos = _Map.POO;
			GACurrent.ParentX = -1;
			GACurrent.ParentY = -1;

			for (int j = 0; j < MoveLimit; j++)
			{
				PopulationList[i][j] = ChildrenList[i][j];
			}
		}
	} while (DestFound == false); //Until the end point of the map is found
}

void GADrawPath(std::vector<GANode> &PathToEnd, struct GANode &GACurrent, SDL_Renderer *_renderer)
{
	SDL_SetRenderDrawColor(_renderer, 0, 0, 255, 255);

	SDL_Rect PathTile;
	PathTile.w = RECTSIZE;
	PathTile.h = RECTSIZE;

	GANode NewGACurrent;

	int PathToEndSize = PathToEnd.size();

	for (int C = 0; C < PathToEndSize; C++) //As I realised there was no point to flipping the path just to have it as a path from start to end, I simply just draw the path as found
	{
		NewGACurrent = *(PathToEnd.rbegin() + C);
		PathTile.x = PathTile.w * NewGACurrent.Pos.X;
		PathTile.y = PathTile.h * NewGACurrent.Pos.Y;
		SDL_RenderFillRect(_renderer, &PathTile);
		SDL_RenderPresent(_renderer);
	}
}
