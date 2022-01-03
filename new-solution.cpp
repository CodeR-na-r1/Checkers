#pragma once

#include <iostream>
#include <fstream>

#include "Board.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board;

	ofstream out;
	out.open("output.txt");
	out << *board;
	out.close();

	return 0;
}