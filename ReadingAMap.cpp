// ReadingAMap.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <fstream>
using namespace std;

int main()
{
	int NumberX;
	int GridX = 0;
	int GridY = 0;

	ifstream inFile1;

	inFile1.open("Lab7TerrainFiles\\Lab7TerrainFile1.txt");
	if (!inFile1) 
	{
		cout << "File was unable to open";
		exit(1);
	}

	if (inFile1)
	{
		inFile1.seekg(0, inFile1.end);
		int inFile1Length = inFile1.tellg();
		inFile1.seekg(0, inFile1.beg);

		char Begg[0];
		GridX = Begg[0];
		cout << "GridX is: " << GridX << endl;
	}

}


