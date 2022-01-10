#pragma once

#include <iostream>
#include <fstream>

#include "Board.h"

int main()
{
	Board* board = new Board("data_in.txt");
	
	cout << *board << endl << endl;
	
	bool who_move = true;
	int draw = 0;
	
	ofstream out;
	out.open("notation_output.txt");

	while (true)
	{
		Point_2D move = board->get_effective_move(who_move);
		board->notation(move, cout);
		board->notation(move, out);
		board->do_move(move);

		if (board->end_game())
			break;

		who_move = !who_move;
		cout << *board << endl << endl;
		if (++draw > 31)
			break;
	}

	out.close();

	cout << *board << endl << endl;

	if (draw > 31)
		cout << "no one won! (draw)\n";
	else
		board->white_is_win() ? cout << "White is win!\n" : cout << "Black is win!\n";

	out.open("output.txt");
	out << *board;
	out.close();

	return 0;
}