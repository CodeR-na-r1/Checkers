#pragma once

#include "Figure.h"
#include "Point_2D.h"
#include "Checher.h"

#include <fstream>
#include <string>
#include <vector>

using namespace std;

class Board
{
public:
	Figure*** board;
	
	Board();
	Board(const char* namefile);

	Point_2D get_effective_move(const bool _isWhite);

	friend ostream& operator<<(ostream& out, const Board& board);

	friend ofstream& operator <<(ofstream& out, const Board& board);

	~Board();
};