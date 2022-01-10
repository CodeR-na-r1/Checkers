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

	bool who_move = true;
	int draw = 0;

	while (true)
	{
		Point_2D move = board->get_effective_move(who_move);
		board->notation(move);
		board->do_move(move);

		if (board->end_game())
			break;

		who_move = !who_move;
		cout << *board << endl << endl;
		if (++draw > 31)
			break;
	}

	cout << *board << endl << endl;

	if (draw > 31)
		cout << "no one won! (draw)\n";
	else
		board->white_is_win() ? cout << "White is win!\n" : cout << "Black is win!\n";

	return 0;
}