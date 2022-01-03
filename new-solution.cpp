#include <iostream>
#include <fstream>

#include "Board.h"
#include "Checher.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board;

	ofstream out;
	out.open("output.txt");
	out << *board;

	return 0;
}