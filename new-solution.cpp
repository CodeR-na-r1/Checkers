#pragma once

#include <iostream>
#include <fstream>

#include "Board.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board << endl << endl;

	ofstream out;
	out.open("output.txt");
	out << *board;
	out.close();

	Point_2D move = board->get_effective_move(0);
	board->do_move(move);
	cout << *board;

	return 0;
}